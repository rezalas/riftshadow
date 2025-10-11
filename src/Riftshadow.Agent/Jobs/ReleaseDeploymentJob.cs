
using Microsoft.Extensions.Options;

using Octokit;

using Quartz;

using Riftshadow.Agent.Configuration;
using Riftshadow.Agent.Packages;

namespace Riftshadow.Agent.Jobs;

/// <summary>
/// The background job for fetching assets from the latest release of riftshadow
/// down to an environment for storage or deployment
/// </summary>
/// <param name="logger"></param>
/// <param name="github"></param>
/// <param name="options"></param>
/// <param name="packages"></param>
[DisallowConcurrentExecution]
public class ReleaseDeploymentJob(ILogger<ReleaseDeploymentJob> logger,
                                  IGitHubClient github,
                                  IOptionsMonitor<RiftshadowAgentOptions> options,
                                  IPackageService packages) : IJob
{
    public static readonly JobKey Key = new JobKey(nameof(ReleaseDeploymentJob));

    public async Task Execute(IJobExecutionContext context)
    {
        logger.LogInformation("Release Deployment Job started at {time}", DateTimeOffset.Now);

        var releaseTag = context.MergedJobDataMap.GetString("releaseTag");
        var releaseId = context.MergedJobDataMap.GetLong("releaseId");
        if (string.IsNullOrEmpty(releaseTag))
        {
            logger.LogWarning("No release tag provided. Skipping deployment.");
            return;
        }

        var releaseAssets = await github.Repository.Release.GetAllAssets("rezalas", "riftshadow", releaseId);
        if (releaseAssets == null || releaseAssets.Count == 0)
        {
            logger.LogWarning("No assets found for release {releaseTag}. Skipping deployment.", releaseTag);
            return;
        }

        var gamePackage = GetAsset(releaseAssets, options.CurrentValue.GamePackagePrefix);
        if (gamePackage is not null)
        {
            await FetchPackageAsync("game", releaseTag, gamePackage, context.CancellationToken);
        }

        var agentPackage = GetAsset(releaseAssets, options.CurrentValue.AgentPackagePrefix);
        if(agentPackage is not null)
		{
            await FetchPackageAsync("agent", releaseTag, agentPackage, context.CancellationToken);
		}

        logger.LogInformation("Release Deployment Job finished at {time}", DateTimeOffset.Now);
    }

    private ReleaseAsset? GetAsset(IReadOnlyList<ReleaseAsset>? assets, string packagePrefix)
	{
		return assets?.FirstOrDefault(a => a.Name.StartsWith(packagePrefix, StringComparison.OrdinalIgnoreCase));
	}

    private async Task FetchPackageAsync(string packageKey, string releaseTag, ReleaseAsset? asset, CancellationToken cancellationToken)
	{
		if (asset is not null)
        {
            logger.LogInformation("Found {key} package {packageName} for release {releaseTag}.", packageKey, asset.Name, releaseTag);

            var releasePath = $"{options.CurrentValue.ReleaseDownloadPath}/{releaseTag}";

            var fetchRequest = new PackageFetchRequest
            {
                PackageName = asset.Name,
                ReleaseTag = releaseTag,
                DownloadFromPath = asset.BrowserDownloadUrl,
                DownloadToPath = Path.Combine(releasePath, asset.Name)
            };

            var fetchResult = await packages.FetchPackageAsync(fetchRequest, cancellationToken);
            if (!fetchResult.Success)
            {
                logger.LogError("Failed to fetch {key} package {packageName} for release {releaseTag}: {message}", packageKey, asset.Name, releaseTag, fetchResult.Message);
                return;
            }
        }
        else
        {
            logger.LogWarning("No {key} package found for release {releaseTag}. Skipping deployment.", packageKey, releaseTag);
        }
	}
}