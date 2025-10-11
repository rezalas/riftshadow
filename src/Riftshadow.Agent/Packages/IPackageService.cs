
namespace Riftshadow.Agent.Packages;

/// <summary>
/// Service responsible for fetching the package files and saving them 
/// to a local location.
/// </summary>
public interface IPackageService
{
    /// <summary>
    /// Fetches a package from a release and downloads it to a local path
    /// </summary>
    /// <param name="request">
    /// The request with information on which package to download,
    /// where to find it, and where to save it.
    /// </param>
    /// <param name="cancellationToken">The cancellation token for the task</param>
    /// <returns>The result indicating whether the fetch succeeded</returns>
    Task<Result> FetchPackageAsync(PackageFetchRequest request, CancellationToken cancellationToken);
    // Task<Result> InstallPackageAsync(string packagePath, string installPath, CancellationToken cancellationToken);

    /// <summary>
    /// Checks to see if a specific release package exists under the release folder
    /// </summary>
    /// <param name="releaseTag">The identifying tag of a release</param>
    /// <param name="packagePrefix">The beginning of the file name for the requested package</param>
    /// <returns>The result indicating whether the requested package exists</returns>
    Result<bool> PackageExists(string releaseTag, string packagePrefix);

    /// <summary>
    /// Checks to see if the release folder exists by releaseTag
    /// </summary>
    /// <param name="releaseTag">The identifying tag of a release</param>
    /// <returns>The result indicating whether the release folder exists</returns>
    Result<bool> ReleaseExists(string releaseTag);
}