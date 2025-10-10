
using Quartz;

namespace Riftshadow.Agent.Jobs;

public class DownloadPackageJob : IJob
{
    public static readonly JobKey Key = new JobKey(nameof(DownloadPackageJob));

    public Task Execute(IJobExecutionContext context)
    {
        var jobData = context.MergedJobDataMap.GetValueOrDefault<DownloadPackageJobData>("jobData");
        
        throw new NotImplementedException();
    }
}
