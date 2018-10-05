## Basics
When you work on anything please create an issue for it or comment on the appropriate issue to avoid duplication of work. 
Please be descriptive in your issues, posts, and PRs and describe what you are trying to achieve.

## Commenting
Not everything needs to be commented, but complex topics should have comments added to ease reading them. Try to be "self documenting"
by using descriptive variable names, short function bodies, and adding spacer lines between ideas where appropriate.

## Code Style
Generally this isn't much of an issue, but try to be consistent and avoid sloppy work. One of the goals is to achieve reasonably 
easy to read code to extend the life of the code base. As such:
* Keep lines a reasonable length, typically under 120 characters unless absolutely needed. 
* Name things reasonably. If you need a guide: https://docs.microsoft.com/en-us/dotnet/standard/design-guidelines/general-naming-conventions
* Put braces on their own lines:
    ~~~~cpp
    public void Something()
    {
        // code
    }
    ~~~~
