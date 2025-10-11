
using Octokit;

using Quartz;

using Riftshadow.Agent.Jobs;

/// <summary>
/// The background job responsible for monitoring riftshadow releases
/// </summary>
/// <param name="logger"></param>
/// <param name="scheduler"></param>
/// <param name="github"></param>
[DisallowConcurrentExecution]
public class ReleaseListenerJob(ILogger<ReleaseListenerJob> logger, IGitHubClient github) : IJob
{
    public static readonly JobKey Key = new JobKey(nameof(ReleaseListenerJob));
    public async Task Execute(IJobExecutionContext context)
    {
        logger.LogInformation("Release Listener Job started at {time}", DateTimeOffset.Now);

        try
        {
            var latestRelease = await github.Repository.Release.GetLatest("rezalas", "riftshadow");
            logger.LogInformation("Latest release: {tag}", latestRelease.TagName);

            var deployJob = JobBuilder.Create<ReleaseDeploymentJob>()
                                      .WithIdentity(ReleaseDeploymentJob.Key)
                                      .UsingJobData("releaseTag", latestRelease.TagName)
                                      .UsingJobData("releaseId", latestRelease.Id)
                                      .Build();

            logger.LogInformation("Scheduling deployment job for release {tag}", latestRelease.TagName);
            await context.Scheduler.ScheduleJob(deployJob, TriggerBuilder.Create().StartNow().Build());
        }
        catch (NotFoundException)
        {
            // Octokit does not return a result if the target repo has no
            // releases. Instead it simply throws a NotFoundException and so
            // this is where we find ourselves.
            logger.LogWarning("No releases found for rezalas/riftshadow repository.");
        }
        catch (Exception ex)
        {
            logger.LogError(ex, "An error occurred while checking for new releases.");
        }

        logger.LogInformation("Release Listener Job finished at {time}", DateTimeOffset.Now);
    }
}