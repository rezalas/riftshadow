
using Octokit;

using Quartz;

using Riftshadow.Agent.Jobs;

[DisallowConcurrentExecution]
public class ReleaseListenerJob(ILogger<ReleaseListenerJob> _logger, IScheduler _scheduler) : IJob
{
    public static readonly JobKey Key = new JobKey(nameof(ReleaseListenerJob));
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

                var deployJob = JobBuilder.Create<ReleaseDeploymentJob>()
                                          .WithIdentity(ReleaseDeploymentJob.Key)
                                          .UsingJobData("releaseTag", latestRelease.TagName)
                                          .Build();

                _logger.LogInformation("Scheduling deployment job for release {tag}", latestRelease.TagName);
                await _scheduler.ScheduleJob(deployJob, TriggerBuilder.Create().StartNow().Build());
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