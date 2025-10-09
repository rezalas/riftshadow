using Quartz;

using Riftshadow.Agent.Jobs;

var builder = Host.CreateApplicationBuilder(args);

builder.Services.AddQuartz(config =>
{
    config.AddJob<ReleaseListenerJob>(opts => opts.WithIdentity(ReleaseListenerJob.Key));

    config.AddTrigger(opts => opts
        .ForJob(ReleaseListenerJob.Key)
        .WithIdentity($"{nameof(ReleaseListenerJob)}-trigger")
        .WithCronSchedule("0 0/1 * * * ?")); // Every minute
});

builder.Services.AddQuartzHostedService(opts =>
{
    opts.WaitForJobsToComplete = true;
});

var host = builder.Build();
host.Run();
