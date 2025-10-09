using Quartz;

var builder = Host.CreateApplicationBuilder(args);

builder.Services.AddQuartz(config =>
{
    config.AddJob<ReleaseListenerJob>(opts => opts.WithIdentity(nameof(ReleaseListenerJob)));

    config.AddTrigger(opts => opts
        .ForJob(nameof(ReleaseListenerJob))
        .WithIdentity($"{nameof(ReleaseListenerJob)}-trigger")
        .WithCronSchedule("0 0/1 * * * ?")); // Every minute
});

builder.Services.AddQuartzHostedService(opts =>
{
    opts.WaitForJobsToComplete = true;
});

var host = builder.Build();
host.Run();
