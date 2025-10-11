
namespace Riftshadow.Agent.Packages;

public interface IPackageService
{
    Task<Result> FetchPackageAsync(PackageFetchRequest request, CancellationToken cancellationToke);
    // Task<Result> InstallPackageAsync(string packagePath, string installPath, CancellationToken cancellationToken);
}