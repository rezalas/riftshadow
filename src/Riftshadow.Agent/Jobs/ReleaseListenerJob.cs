
using Quartz;

[DisallowConcurrentExecution]
public class ReleaseListenerJob(ILogger<ReleaseListenerJob> _logger) : IJob
{
    public async Task Execute(IJobExecutionContext context)
    {
        _logger.LogInformation("ReleaseListenerJob started at {time}", DateTimeOffset.Now);
        try
        {

        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "An error occurred while checking for new releases.");
        }
        
        _logger.LogInformation("ReleaseListenerJob finished at {time}", DateTimeOffset.Now);
    }
}