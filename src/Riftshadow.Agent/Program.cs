using Octokit;

using Quartz;
using Riftshadow.Agent.Packages;

using Riftshadow.Agent.Configuration;

#if LINUX
using Microsoft.Extensions.Hosting.Systemd;
#endif

var builder = Host.CreateApplicationBuilder(args);

#if LINUX
    builder.UseSystemd();
#endif

#if WINDOWS && !DEBUG
    builder.Services.AddWindowsService(options =>
    {
        options.ServiceName = "Riftshadow Deployment Agent";
    });
#endif

builder.Configuration.AddEnvironmentVariables(prefix: "RIFT_");

builder.Services.AddQuartz(config =>
{
    config.AddJob<ReleaseListenerJob>(opts => opts.WithIdentity(ReleaseListenerJob.Key));

    var scanCron = builder.Configuration.GetSection(RiftshadowAgentOptions.SectionName)[nameof(RiftshadowAgentOptions.ReleaseScanCron)] ?? "0 0/1 * * * ?";

    config.AddTrigger(opts => opts
        .ForJob(ReleaseListenerJob.Key)
        .WithIdentity($"{nameof(ReleaseListenerJob)}-trigger")
        .WithCronSchedule(scanCron)); // Every minute
});

builder.Services.AddQuartzHostedService(opts =>
{
    opts.WaitForJobsToComplete = true;
});

builder.Services.AddTransient<IGitHubClient, GitHubClient>((sp) =>
{
    var client = new GitHubClient(new ProductHeaderValue("riftshadow-agent"));

    var token = builder.Configuration["GITHUB_TOKEN"];
    if (!string.IsNullOrEmpty(token))
    {
        var tokenAuth = new Credentials(token);
        client.Credentials = tokenAuth;
    }

    return client;
});

builder.Services.AddTransient<IPackageService, PackageService>();

builder.Services.Configure<RiftshadowAgentOptions>(builder.Configuration.GetSection(RiftshadowAgentOptions.SectionName));

var host = builder.Build();
host.Run();
