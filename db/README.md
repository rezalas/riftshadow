## Why Single files and ALSO folders?

You may find yourself in the unfortunate situation that you need to be poking around in this directory. 

If that is you, first of all, welcome! Secondly, there is a reason to have the SQL files `setup`, `rift`, and `rift_core` here while also having them split up in folders and files of their own.

The reason for this is that we originally need to have these backup files split up into a more readable form to have better visibility on the structure of the databases. However, once they became the way that the `dev-install.sh` script restore the development databases, very important pieces of the database puzzle were lost and development environments could no longer load Areas.

In order to keep the visibility of table structure and the wait for it... _usability_ of the dev databases they exist together in quasi-harmony.

Cheers!