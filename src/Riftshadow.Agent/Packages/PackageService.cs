using System.Security.Cryptography.X509Certificates;

using Microsoft.Extensions.Options;

using Riftshadow.Agent.Configuration;

namespace Riftshadow.Agent.Packages;

/// <inheritdoc cref="IPackageService" />
public class PackageService(ILogger<PackageService> logger, IOptionsMonitor<RiftshadowAgentOptions> options) : IPackageService
{
    public async Task<Result> FetchPackageAsync(PackageFetchRequest request, CancellationToken cancellationToken)
    {
        logger.LogInformation("Fetching package {packageName} from {downloadFromPath} to {downloadToPath}", request.PackageName, request.DownloadFromPath, request.DownloadToPath);

        if (!Directory.Exists(request.DownloadToPath))
        {
            logger.LogInformation("Creating directory {downloadToPath}", request.DownloadToPath);
            Directory.CreateDirectory(request.DownloadToPath);
        }

        var http = new HttpClient();

        var response = await http.GetAsync(request.DownloadFromPath, cancellationToken);

        if (!response.IsSuccessStatusCode)
        {
            logger.LogError("Failed to download package {packageName} from {downloadFromPath}. Status code: {statusCode}", request.PackageName, request.DownloadFromPath, response.StatusCode);
            return new Result(false, $"Failed to download package {request.DownloadFromPath}. Status code: {response.StatusCode}");
        }

        string filename = Path.GetFileName(request.DownloadToPath);
        await using var fs = File.Create(request.DownloadToPath);
        await response.Content.CopyToAsync(fs);

        logger.LogInformation("Downloaded package {packageName} to {downloadToPath}", request.PackageName, request.DownloadToPath);
        return new Result(true, $"Package {filename} downloaded successfully.");
    }

    public Result<bool> PackageExists(string releaseTag, string packagePrefix)
    {
        var releaseResult = ReleaseExists(releaseTag);

        if (releaseResult.Success == false)
            return releaseResult;

        string path = Path.Combine(options.CurrentValue.ReleaseDownloadPath, releaseTag);

        var file = Directory.EnumerateFiles(path).FirstOrDefault(x => x.StartsWith(packagePrefix, StringComparison.InvariantCultureIgnoreCase));

        if (string.IsNullOrWhiteSpace(file))
            return new("Package not found", false);

        return new($"Package {Path.GetFileName(file)} found", true);
    }
    
    public Result<bool> ReleaseExists(string releaseTag)
	{
        if (string.IsNullOrWhiteSpace(releaseTag))
            return new Result<bool>("'releaseTag' cannot be null or whitespace.", false);

        string path = Path.Combine(options.CurrentValue.ReleaseDownloadPath, releaseTag);
        bool exists = Directory.Exists(path);

        return new($"{(exists ? "Found" : "No")} directory {path}", exists);
	}

    // public Task<Result> InstallPackageAsync(string packagePath, string installPath, CancellationToken cancellationToken)
    // {
    //     throw new NotImplementedException();
    // }
}