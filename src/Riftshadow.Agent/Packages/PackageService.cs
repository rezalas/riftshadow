using Riftshadow.Agent;
using Microsoft.Extensions.Logging;

namespace Riftshadow.Agent.Packages;

public class PackageService(ILogger<PackageService> logger) : IPackageService
{
    public Task<Result> FetchPackageAsync(PackageFetchRequest request, CancellationToken cancellationToken)
    {
        logger.LogInformation("Fetching package {packageName} from {downloadFromPath} to {downloadToPath}", request.PackageName, request.DownloadFromPath, request.DownloadToPath);
        
        if (!Directory.Exists(request.DownloadToPath))
		{
            logger.LogInformation("Creating directory {downloadToPath}", request.DownloadToPath);
            Directory.CreateDirectory(request.DownloadToPath);
		}

        var response = await new HttpClient().GetAsync(request.DownloadFromPath, cancellationToken);

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

    public Task InstallPackageAsync(string packagePath, string installPath)
    {
        throw new NotImplementedException();
    }
}