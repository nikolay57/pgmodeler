Changelog
---------

v0.4.0_r1
---------

<em>Release date: March 04, 2013 </em>

[New]: introducing the "pgModeler Wiki" as the main project's support resource.
[Fix]: when main windows is closed the overview widget is closed too.
[Fix]: corrected a bug on operation list widget that was converting an item name to UTF-8 twice.

v0.4.0
------

<em>Release date: February 27, 2013 </em>

* [New]: introduce a "New object" submenu when activating the schema context menu (right-click)
* [New]: tables and view are now graphically separated by colored rectangles representing its schemas.
* [New]: compiling pgModeler now works perfectly on Mac OSX system.
* [New]: introduced the 'Quick actions' menu that permits: rename, move to another schema, change onwer and edit permissions.
* [New]: the relationship editing form gained an "advanced" tab which shows the objects generated and/or represents the relatioship itself.
* [New]: the user now can add relationships only creating foreign keys on tables (fk relationships).
* [New]: added a french UI translation (provided by [toorpy](https://github.com/toorpy)).
* [Change]: all relationships type are now grouped together on "Model objects" widget.
* [Change]: chinese UI translation updated (provided by: [Bumanji](https://github.com/Bumanji)).
* [Change]: user now can remove fk relationships directly without needing to remove the related foreign keys.
* [Change]: field semantics adjustments on relationship editing form.
* [Change]: graphical object can be now selected and have the context menu activated only with a single right-click.
* [Change]: minor improvements on plugin base class: PgModelerPlugin.
* [Change]: widget size adjustments to better showing on Mac OSX system.
* [Change]: crashhandler now shows the compiled and running versions of Qt.
* [Change]: french UI translation reviewed and updated (provided by [babs](https://github.com/babs)).
* [Change]: 'Objects of Model' when used as object picker now expand all the nodes by default.
* [Change]: 'Objects of Model' now memorizes the tree state when update an object and / or opening another model.
* [Change]: PostGiS 'geometry' type can have a free assigned SRID value.
* [Change]: editing forms when shown set the focus on the first field, generally, the object name.
* [Change]: 'Objects of Model' widget displays the nodes in alphabetical order.
* [Change]: the printing options for the model were moved to the general configuration form.
* [Change]: relationship validation method now removes fk relationships when the foreign keys that gerenates is no longer exists.
* [Change]: copy/cut/delete commands does not manipulates system objects like schema public and languages C, SQL and plpgsql.
* [Change]: pgModeler startup scripts are now path location free meaning that software can be installed where the user desires.
* [Fix]: corrected a bug related  constraint name on domain XML code generation.
* [Fix]: corrected a bug that was causing crash when click "Apply" on Type editing form with fields not filled.
* [Fix]: corrected the "invalid constraint name" error on domain editing form.
* [Fix]: corrected the empty DEFAULT clause for columns, types and domains.
* [Fix]: corrected a bug related to incorrectly initialized OID attribute when creating tables.
* [Fix]: corrected a bug when creating a view with WHERE statement.
* [Fix]: corrected a bug related to one-to-many relationships semantics.
* [Fix]: corrected some bugs that was causing crash when removing all operations from operation list.
* [Fix]: minor bug fixes related to object selection over the model.
* [Fix]: corrected a bug on load model dialog filter (chinese UI only).
* [Fix]: pgModeler no longer crashes when editing objects style.
* [Fix]: corrected bug that was deleting two sequeces at once.
* [Fix]: pgModeler no longer crashes when removing (disconnecting) relationship that has special primary keys.
* [Fix]: minor fixes on the startup scripts on all platforms.
* [Fix]: corrected an incorrect reference to output stream on Windows system.
* [Fix]: shortcuts and popup menu now works correctly when selection an object on 'Objects of Model' tree.
* [Fix]: the pgsql base types (represented by tables, sequences, user defined types and domains) are now updated correctly when the related schema is renamed.
* [Fix]: corrected some weird SRID value on non spatial types.
* [Fix]: corrected bug on objects table when move rows to last / first.
* [Fix]: typos corrections on some error messages and dialog titles.
* [Fix]: 'referenced columns' combobox on constraint editing form are filled correctly when the dialog is shown in a second time.
* [Fix]: pgModeler no longer crashes when creating many-to-many relationships.
* [Fix]: pgModeler no longer crashes when the user activates the print dialog.
* [Fix]: corrected bug that was removing fk relationships when pasting objects.
* [Fix]: corrected SQL syntax error of 'timestamp with time zone'.
* [Fix]: corrected constraint type showing on editing form.
* [Fix]: corrected bug on cyrillic typed enums and check constraints expressions.
* [Fix]: corrected bug on enumeration type editing form.
* [Fix]: corrected bug on 'truncate' table privilege code generation.
* [Fix]: corrected column default value code generation.
* [Fix]: dummyplugin build process corrected on Windows.
* [Fix]: corrected bug on column comment code generation.
* [Fix]: corrected bug that was deleting two tables at once.

v0.3.4
------

<em>Release date: October 17, 2012</em>

* [New]: added chinese UI translation (provided by [gjunming](https://github.com/gjunming)).
* [New]: added basic support for PostGiS 2.0 only data types: box2d, box3d, geometry and geography (suggested by [george-silva](https://github.com/george-silva) on [issue#28](https://github.com/pgmodeler/pgmodeler/issues/28))(EXPERIMENTAL). Note: when using these data types make sure that PostGiS extension is installed on database cluster since pgModeler WILL NOT install it automatically or generate the command to do it!
* [New]: added a model restoration feature to reopen models after unexpected quit (crash).
* [New]: added a crash handler to pgModeler. Now signal SIGSEGV is trapped (in most cases) and the crash handler pops up permiting the user to generate an error report. (EXPERIMENTAL)
* [New]: to facilitate the error reporting exceptions stack now can be showed in text format. Users can post the complete error stack when creating an issue.
* [New]: icon added to pgModeler executable (Windows only)
* [Change]: update on pt_BR translation file.
* [Change]: removed "pgmodeler" prefix from translation files.
* [Change]: added the field "Underline" on textbox editing form.
* [Fix]: corrected the "AlwayOnTop" bug on model overview widget. ([issue#30](https://github.com/pgmodeler/pgmodeler/issues/30))
* [Fix]: little fix on startup scripts. Corrected de PGMODELER_ROOT on both Linux and Windows systems. ([issue#29](https://github.com/pgmodeler/pgmodeler/issues/29))
* [Fix]: corrected the referece to environment variables PGMODELER_*. Now pgModeler search for necessary paths on current directory if some of these variables are not set.
* [Fix]: corrected the validation of UTF-8 names that have 3 bytes length.
* [Fix]: corrected the sources path reference on project (.pro) files. Now lupdate command do not generates empty TS files.
* [Fix]: corrected a bug that was causing crash where user try to edit protected objects.
* [Fix]: corrected the exhibition of UTF-8 messages on ```throw``` statements.

v0.3.3
------

<em>Release date: October 09, 2012</em>

* [Change]: pgModeler license were update to GPLv3.
* [Change]: Error massages and entire UI were translated to en_US. Now people can contribute more easily with translation files. [(issue#8)](https://github.com/pgmodeler/pgmodeler/issues/8)
* [Change]: The left side image were removed form all forms giving more space to show widgets.
* [Change]: pgModeler now shows a messagebox at startup if any critical error is raised instead to show them on stdin.
* [Fix]: Translation files now are correctly loaded depending on system language. [(issue#23)](https://github.com/pgmodeler/pgmodeler/issues/23)
* [Fix]: Compilation process and execution is working correctly on Windows system. [(issue#11)](https://github.com/pgmodeler/pgmodeler/issues/11)
* [Fix]: No more crashes when dealing with relationships that have special triggers/indexes/columns. [(issue#8)](https://github.com/pgmodeler/pgmodeler/issues/8) [(issue#24)](https://github.com/pgmodeler/pgmodeler/issues/24)

v0.3.2
------

<em>Release date: September 27, 2012</em>

* [Change]: The default extension for the models now stands for ".dbm" [(issue#9)](https://github.com/pgmodeler/pgmodeler/issues/9)
* [Change]: Tables and sequences now can be used as function return type as well parameter type. This is valid for other objects that make use of base types (except for table columns).
* [Change]: The relationship conversion command now need to be confirmed by the user.
* [Fix]: Compilation process now works correctly on Windows system.
* [Fix]: Adjusted the size of some forms to show their fields properly.
* [Fix]: The "make distclean" command now make the correct cleanup on build/ directory.
* [Fix]: Startup scripts "start-pgmodeler.(sh|bat)" where adjusted. To prevent errors pgModeler need to be started through these scripts.
* [Fix]: Corrected the reference to the plugins directory. [(issue#7)](https://github.com/pgmodeler/pgmodeler/issues/7)
* [Fix]: The action "New Object -> Tablespace" now is displayed properly.

v0.3.1
------

<em>Release date: September 18, 2012</em>

* [New]: Relationships generates column suffixes automaticaly. This behavior can be changed on the relationship editing form.
* [New]: Added two samples to pgModeler.
* [Change]: Tables are now created with "With OIDs" attribute by default.
* [Change] The graphical update method on overview widget has improved preventing unecessary processing.
* [Fix]: Class CenaObjetos now doesn't delete objects twice.
* [Fix]: Eliminated bug that caused crashing on pgModeler when closing a model.

v0.3.0
------

<em>Release date: September 12, 2012</em>

* [New]: Added a model overview widget.
* [New]: Added export feature that generates PNG image of the models.
* [Fix]: Corrected the naming of columns generated by many-to-many relationships.
* [Fix]: Corrected generation of XML/SQL code by the model.

v0.2.0
------

<em>Release date: August 31, 2012</em>

* [New]: Added an interface to implement third party plugins. Check [PLUGINS.md] (https://github.com/pgmodeler/pgmodeler/blob/master/PLUGINS.md) for details.
* [New]: Added a short cut to easily control the zoom on the model. Use Crtl + Mouse wheel up (zoom up) or Crtl + Mouse wheel down (zoom down).
* [Change]: Due to the plugin interface the compilation method changed back to the form of shared libraries + executable.
* [Fix]: No more crashes when removing an primary-key of a table which has relationship with other tables. [(issue#2)](https://github.com/pgmodeler/pgmodeler/issues/2)
* [Fix]: Adjusted the semantics of one-to-one relationships.

v0.1.2
------

<em>Release date: August 24, 2012</em>

* [New]: Added a functionality to save modified models before closing the software.
* [Change]: Updated the en_US dictionary with the texts of the above functionality.
* [Fix]: Dockwidgets no longer disappear unexpectedly when the main window is minimized.
* [Fix]: Operations performed before creating a table object (column, constraint, trigger, index, rule) are no longer removed when any exception is thrown in the creation of these object.
* [Fix]: Fixed bug that caused user-defined types had wrong SQL/XML code generated by the model.
* [Fix]: Functions and Types received an own range of id in order to create these objects in a correct way.
* [Fix]: Eliminated segmentation faults caused by the destruction of relationships which possessed attributes/constraints.
* [Fix]: Adjusted the translation to SQL code of one-to-one relationships.
* [Fix]: Eliminated segmentation fault when editing relationships and/or undoing an operation involving a relationship.
* [Fix]: Identifiers relationships now correctly display the thick line beside the weak entity.

v0.1.1
------

<em>Release date: August 14, 2012</em>

* [Fix]: Correction of the actions for inserting graphic objects (table, text box, vision and relationship) in Windows environment.
* [Fix]: Correction on the display of the maximize button in the window decoration in Windows environment.
* [Fix]: Adjust on the position and spacing of widgets in editing forms.
* [Fix]: The XML parser can now correctly read DTD files in Windows environment.
* [Fix]: The compilation method is no longer in the form of shared libraries + executable and passed to be as standalone executable only.

v0.1.0
------

<em>Release date: August 9, 2012</em>

* First pgModeler release.
