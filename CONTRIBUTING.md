## Basics
When you work on anything please create an issue for it or comment on the appropriate issue to avoid duplication of work. 
Please be descriptive in your issues, posts, and PRs and describe what you are trying to achieve.

## Commenting
Not everything needs to be commented, but complex topics should have comments added to ease reading them. Try to be "self documenting"
by using descriptive variable names, short function bodies, and adding spacer lines between ideas where appropriate. If you think someone might wonder 'why' something was done a certain way and you have a good reason, that's a good spot for  a friendly comment. 

## Optimization
Though the goal of modernizing will include optimization, avoid premature optimization without reason. The program is quite large and old, and as such some things may react poorly to optimizing in ways that are not easily noticed. If you optimize code, be certain the function and make sure to spin up a server instance and validate your changes.

## Code Style
Generally this isn't much of an issue, but try to be consistent and avoid sloppy work. One of the goals is to achieve reasonably 
easy to read code to extend the life of the code base. As such:
* Indent with tabs, not spaces. Many Bothans died to bring us this rule.
* If you work on the website/PHP section, please follow PSR-12 but do note the tabs instead of spaces https://www.php-fig.org/psr/psr-12/
* For definitions, use `#ifdef ...` when one condition is required.
* Follow modern C++ convention, properly utilizing header files.
* Try to stay SOLID and DRY (which can be difficult in a 20yr old code base).
* Keep lines a reasonable length, typically under 120 characters unless absolutely needed. 
* Name things reasonably. If you need a guide: https://docs.microsoft.com/en-us/dotnet/standard/design-guidelines/general-naming-conventions
* Put braces on their own lines:
    ~~~~cpp
    public void Something()
    {
        // code
    }
    ~~~~
