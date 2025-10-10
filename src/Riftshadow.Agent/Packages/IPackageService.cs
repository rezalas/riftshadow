
namespace Riftshadow.Agent.Packages;

public interface IPackageService
{
    Task FetchPackageAsync(PackageFetchRequest request, CancellationToken cancellationToke);
    Task InstallPackageAsync(string packagePath, string installPath);
}