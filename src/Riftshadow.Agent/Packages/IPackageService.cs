
namespace Riftshadow.Agent.Packages;

/// <summary>
/// Service responsible for fetching the package files and saving them 
/// to a local location.
/// </summary>
public interface IPackageService
{
    Task<Result> FetchPackageAsync(PackageFetchRequest request, CancellationToken cancellationToke);
    // Task<Result> InstallPackageAsync(string packagePath, string installPath, CancellationToken cancellationToken);
}