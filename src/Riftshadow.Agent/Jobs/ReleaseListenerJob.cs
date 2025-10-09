
using Octokit;

using Quartz;

[DisallowConcurrentExecution]
public class ReleaseListenerJob(ILogger<ReleaseListenerJob> _logger) : IJob
{
    public async Task Execute(IJobExecutionContext context)
    {
        _logger.LogInformation("Release Listener Job started at {time}", DateTimeOffset.Now);
        
        try
        {
            var github = new GitHubClient(new ProductHeaderValue("riftshadow-agent"));

            try
            {
                var latestRelease = await github.Repository.Release.GetLatest("rezalas", "riftshadow");
                _logger.LogInformation("Latest release: {tag}", latestRelease.TagName);
            }
            catch (NotFoundException)
            {
                _logger.LogWarning("No releases found for rezalas/riftshadow repository.");
                return;
            }
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "An error occurred while checking for new releases.");
        }

        _logger.LogInformation("Release Listener Job finished at {time}", DateTimeOffset.Now);
    }
}