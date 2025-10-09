
using Quartz;

namespace Riftshadow.Agent.Jobs;

[DisallowConcurrentExecution]
public class ReleaseDeploymentJob(ILogger<ReleaseDeploymentJob> _logger) : IJob
{
    public static readonly JobKey Key = new JobKey(nameof(ReleaseDeploymentJob));

    public async Task Execute(IJobExecutionContext context)
    {
        _logger.LogInformation("Release Deployment Job started at {time}", DateTimeOffset.Now);

        var releaseTag = context.MergedJobDataMap.GetString("releaseTag");

        if (string.IsNullOrEmpty(releaseTag))
        {
            _logger.LogWarning("No release tag provided. Skipping deployment.");
            return;
        }

        await Task.Delay(1000); // Simulate work
        _logger.LogInformation("Release Deployment Job finished at {time}", DateTimeOffset.Now);
    }
}