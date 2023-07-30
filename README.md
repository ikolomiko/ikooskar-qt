## A more complete documentation of ikoOSKAR-Qt is now out! Check it out from [here!](https://ikolomiko.github.io/ikooskar-qt/annotated.html)

# Table of Contents
* [What is ikoOSKAR](#what-is-ikooskar)
* [ikoOSKAR-Qt](#ikooskar-qt)
	- [Dependencies for development](#dependencies-for-development)
	- [Dependencies for the end user](#dependencies-for-the-end-user)
* [Documentation Overview](#documentation-overview)
	- [Contents of Layers](#contents-of-layers)
* [License](#license)

&nbsp;

# What is ikoOSKAR
ikoOSKAR (iko Ortak Sınav Karma Sistemi) allows you to easily manage
your school's student placement schemes for exams. This program is especially
targeted for high schools. Check out 
[the official website for ikoOSKAR!](https://ikooskar.web.app) (still WIP)

With ikoOSKAR you can:

* Create student placement schemes with minimum same-grade student interaction
* Get placement schemes for each classroom / exam hall
* Create the list of students' exam places (the classroom / exam hall and the
desk number) for each class / section, respectively
* Prevent students from cheating in exams
* Easily manage the student database
* View and manipulate each class / section
* Add hundreds of new students to the database at once; either grouped by their
classes / sections, or not grouped by any criteria at all
* Add, edit or remove individual students
* Update all students' grade informations at the end of each academic year, with
just one click.
  - This action also assumes that every 12th grade student have graduated and 
proceeds to delete every 12th grade student from the database 
* View and reprint older schemes
* Search and filter older schemes by date or by exam name

&nbsp;

# ikoOSKAR-Qt
This project is a reimplementation of ikoOSKAR which was originally written in 
C# and .Net Framework. Note that this project is still WIP and currently there's
no installers or pre-compiled binaries available.

C++17 and Qt Framework 6.x are being used for this reimplementation, which makes the
program cross-platform and also more efficient.

The code formatter of our choice is `clang-format` with `-style=webkit` styling 
option.

`doxygen` is being used for the auto-generated and auto-deployed documentations, 
with the configurations in [this file](/Doxyfile), using 
[this script](/scripts/generate-docs.sh).

## Dependencies for development
* Qt Creator (optional but highly recommended)
* Qt Framework >= 6.2
* Compiler: GCC (for Linux), MinGW (for Windows)
  - For cross compiling from Linux to Windows, we use
[this script](/scripts/compile4win32.sh) which utilizes [MXE](https://mxe.cc/) 
(with `x86_64-w64-mingw32.static-cmake` package).
  You won't need this if your host OS is Windows.
* CMake
* Ninja (optional)
* Code formatter (optional): `clang-format`

## Dependencies for the end user
* Internet connection for the first activation
* For Windows users: either Microsoft Office or LibreOffice
* For Linux users: LibreOffice

&nbsp;

# Documentation Overview
This project consists of 4 layers:

1. Data Access Layer (DAL for short): This is the layer which interacts with raw
 data; such as the database, the filesystem and the CloudAuth API
2. Business Logic Layer (BLL for short): This is the layer which contains the 
program logic and interacts between the UI and the DAL
3. User Interface Layer (UI for short) (aka Presentation Layer): This is the 
layer which handles all UI operations
4. Shared Layer (aka Entities): This layer contains shared classes and 
information across all the other layers

For further information, check out [the official documentation](https://ikolomiko.github.io/ikooskar-qt/annotated.html)

&nbsp;

## Contents of Layers
You can see which class belongs to which layer in the tables below. Classes with 
direct relationship are represented at the same row. The Shared Layer is shown
separately because its classes are used commonly among other classes and do NOT
have a direct relationship with only one (or two) class(es).

You can click on the elements of the tables to navigate to 
the corresponding file/folder.


|DAL			|BLL				|UI
----------------|-------------------|-----------------------
| [Database](/src/DAL/Database) | [DatabaseHelper](/src/BLL/DatabaseHelper) | [DatabaseUi](/src/UI/DatabaseUi)
| [MultiImport](/src/DAL/MultiImport)	| [MultiImportHelper](/src/BLL/MultiImportHelper)	| [MultiImportUi](/src/UI/MultiImportUi)
| -				| [StudentEditor](/src/BLL/StudentEditor) | [StudentEditorUi](/src/UI/StudentEditorUi)
| -				| SchemeGenerator	| [SchemesUi](/src/UI/SchemesUi)
| XlsxWriter	| SchemeExporter	| -
| -				| AboutHelper 		| AboutUi
| - 			| - 				| [MainPage](/src/UI/MainPage)
| LocalAuth		| LocalAuthHelper 	| -
| CloudAuth		| CloudAuthHelper 	| -
| - 			| Authenticator 	| AuthenticatorPage
| -				| - 				| SplashPage
| -       		| -         		| [ErrorUi](/src/UI/ErrorUi)
| -	            | -                 | [WelcomeUi](/src/UI/WelcomeUi)
| -				| -					| [Module](/src/UI/Common)


|Shared	|
|:-------------:|
|[Student](/src/Shared/student.h)		|
|Desk			|
|Classroom		|
|ExamStudent	|

&nbsp;

# License
ikoOSKAR-Qt is licensed under the [GNU General Public License version 3](/LICENSE) or later.
