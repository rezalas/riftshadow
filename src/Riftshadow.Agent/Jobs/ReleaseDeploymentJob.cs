
using Microsoft.Extensions.Options;

using Octokit;

using Quartz;

using Riftshadow.Agent.Configuration;

namespace Riftshadow.Agent.Jobs;

[DisallowConcurrentExecution]
public class ReleaseDeploymentJob(ILogger<ReleaseDeploymentJob> _logger,
                                  IGitHubClient _github,
                                  IOptionsMonitor<RiftshadowAgentOptions> _options) : IJob
{
    public static readonly JobKey Key = new JobKey(nameof(ReleaseDeploymentJob));

    public async Task Execute(IJobExecutionContext context)
    {
        _logger.LogInformation("Release Deployment Job started at {time}", DateTimeOffset.Now);

        var releaseTag = context.MergedJobDataMap.GetString("releaseTag");
        var releaseId = context.MergedJobDataMap.GetLong("releaseId");

        if (string.IsNullOrEmpty(releaseTag))
        {
            _logger.LogWarning("No release tag provided. Skipping deployment.");
            return;
        }

        var releaseAssets = await _github.Repository.Release.GetAllAssets("rezalas", "riftshadow", releaseId);
        if (releaseAssets == null || releaseAssets.Count == 0)
        {
            _logger.LogWarning("No assets found for release {releaseTag}. Skipping deployment.", releaseTag);
            return;
        }

        var gamePackagePrefix = _options.CurrentValue.GamePackagePrefix;
        var agentPackagePrefix = _options.CurrentValue.AgentPackagePrefix;

        var gamePackage = releaseAssets.FirstOrDefault(a => a.Name.StartsWith(gamePackagePrefix, StringComparison.OrdinalIgnoreCase));
        var agentPackage = releaseAssets.FirstOrDefault(a => a.Name.StartsWith(agentPackagePrefix, StringComparison.OrdinalIgnoreCase));

        if (gamePackage is null && agentPackage is null)
        {
            _logger.LogWarning("No relevant packages found for release {releaseTag}. Skipping deployment.", releaseTag);
            return;
        }

        var releasePath = $"{_options.CurrentValue.ReleaseDownloadPath}/{releaseTag}";
        if (!Directory.Exists(releasePath))        
            Directory.CreateDirectory(releasePath);

        var http = new HttpClient();

        if (gamePackage is not null)
        {
            _logger.LogInformation("Found game package {packageName} for release {releaseTag}.", gamePackage.Name, releaseTag);

            var gamePackageResponse = await http.GetAsync(gamePackage.BrowserDownloadUrl);

            if (gamePackageResponse.IsSuccessStatusCode)
            {
                var gamePackageFilePath = Path.Combine(releasePath, gamePackage.Name);
                await using var fs = File.Create(gamePackageFilePath);
                await gamePackageResponse.Content.CopyToAsync(fs);
                _logger.LogInformation("Downloaded game package to {filePath}.", gamePackageFilePath);
            }
            else
            {
                _logger.LogError("Failed to download game package {packageName} for release {releaseTag}. Status code: {statusCode}", gamePackage.Name, releaseTag, gamePackageResponse.StatusCode);
            }            
        }
        else
		{
            _logger.LogWarning("No game package found for release {releaseTag}. Skipping deployment.", releaseTag);
		}

        if (agentPackage is not null)
        {
            _logger.LogInformation("Found agent package {packageName} for release {releaseTag}.", agentPackage.Name, releaseTag);
        }
        else
		{
            _logger.LogWarning("No agent package found for release {releaseTag}. Skipping deployment.", releaseTag);
		}
        
        _logger.LogInformation("Release Deployment Job finished at {time}", DateTimeOffset.Now);
    }
}