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
ikoOSKAR (**iko** **O**rtak **S**ınav **Kar**ma Sistemi) allows you to easily manage
your school's student placement schemes for exams. This program is especially
targeted for high schools. Check out 
[the official website for ikoOSKAR!](https://ikooskar.com.tr)

With ikoOSKAR you can:

* Create student placement schemes with minimum same-grade student interaction
* Get placement schemes for each classroom / exam hall
* Create the list of students' exam places (the classroom / exam hall and the
desk number) for each class / section, respectively
* Prevent students from cheating in exams
* Easily manage the student database
* View and manipulate each class / section
* Add hundreds of new students to the database at once; just select the 
spreadsheet file exported from e-okul or create your own xlsx file.
* Add, edit or remove individual students
* Update all students' grade informations at the end of each academic year, with
just one click.
  - This action also assumes that every 12th grade student have graduated and 
proceeds to delete every 12th grade student from the database 
* View and edit older schemes, grouped by date

&nbsp;

# ikoOSKAR-Qt
This project is a reimplementation of ikoOSKAR which was originally written in 
C# and .Net Framework.

C++17 and Qt Framework 6.x are being used for this reimplementation, which makes the
program cross-platform and also more efficient.

The code formatter of choice is `clang-format` with `-style=webkit` styling 
option.

`doxygen` is being used for the auto-generated and auto-deployed documentations, 
with the configurations in [this file](/Doxyfile), using 
[this script](/scripts/generate-docs.sh).

## Dependencies for development
* Qt Creator (optional but highly recommended)
* Qt Framework >= 6.2
* Compiler: GCC (for Linux), MinGW-w64 (for Windows)
  - `x86_64-w64-mingw32.static-cmake` package is required for cross compiling
from Linux to Windows. The package is available on [MXE](https://mxe.cc) and
required by [this cross-compiling script](/scripts/compile4win32.sh).
* libexpat
* CMake
* Ninja (optional)
* Code formatter (optional): `clang-format`
* NSIS >= 3.0, for creating installers for Windows
  - `x86_64-w64-mingw32.static-makensis` package is required when creating
Windows installers from Linux. The package is available on [MXE](https://mxe.cc) 
and required by [this cross-compiling script](/scripts/compile4win32.sh).

## Dependencies for the end user
* Internet connection for the first activation
* Any program that can open .xlsx files (e.g. LibreOffice, Gnumeric, WPS Office, MS Office)

&nbsp;

# Documentation Overview
This project consists of 4 layers:

1. Data Access Layer (DAL for short): This is the layer which interacts with raw
 data; such as the database, the filesystem and the [CloudAuth API](https://github.com/ikolomiko/ikooskar-cloudauth)
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
| [Database](/src/DAL/Database) | [DatabaseHelper](/src/BLL/DatabaseHelper) | [DatabasePage](/src/UI/DatabasePage)
| [MultiImport](/src/DAL/MultiImport) <sup>[[1]](#license)</sup>	| [MultiImportHelper](/src/BLL/MultiImportHelper)	| [MultiImportDialog](/src/UI/MultiImportDialog)
| -				| [StudentEditor](/src/BLL/StudentEditor) | [StudentEditorDialog](/src/UI/StudentEditorDialog)
| -				| [SchemeGenerator](/src/BLL/SchemeGenerator)	| [NewSchemeDialog](/src/UI/NewSchemeDialog)
| [libxlsxwriter](https://github.com/jmcnamara/libxlsxwriter) <sup>[[2]](#license)</sup>	| [SchemeExporter](/src/BLL/SchemeExporter)	| -
| -				| [HistoryProvider](/src/BLL/HistoryProvider)	| [SchemesPage](/src/UI/SchemesPage) <sup>[[3]](#license)</sup>
| -				| [Authenticator](/src/BLL/Authenticator) 	| [AboutPage](/src/UI/AboutPage)
| - 			| - 				| [MainWindow](/src/UI/MainWindow)
| [LocalAuth](/src/DAL/LocalAuth), [CloudAuth](/src/DAL/CloudAuth) 			| [Authenticator](/src/BLL/Authenticator) 	| [AuthenticatorWindow](/src/UI/AuthenticatorWindow)
| -       		| -         		| [ErrorUi](/src/UI/ErrorUi)
| -	            | -                 | [WelcomePage](/src/UI/WelcomePage)
| -				| -					| [Page](/src/UI/Common/page.h)


|Shared	|
|:-------------:|
|[Student](/src/Shared/student.h)		|
|[ExamStudent](/src/Shared/examstudent.h)	|
|[Desk](/src/Shared/desk.h)			|
|[DeskCoordinates](/src/Shared/deskcoordinates.h) |
|[Hall](/src/Shared/hall.h)			|
|[Pattern](/src/Shared/pattern.h)		|
|[Scheme](/src/Shared/scheme.h)			|

&nbsp;

# License
ikoOSKAR-Qt is licensed under the [GNU General Public License version 3](/LICENSE) or later.

[1] [FreeXL](https://www.gaia-gis.it/fossil/freexl/home), the library used by `ikoOSKAR::DAL::MultiImport` to parse XLS and XLSX files, is licensed under the [MPL tri-license](http://www.mozilla.org/MPL/boilerplate-1.1/mpl-tri-license-html) terms.
The GPL v2.0+ is chosen for this project.

[2] [libxlsxwriter](https://github.com/jmcnamara/libxlsxwriter), the library used by `ikoOSKAR::BLL::SchemeExporter`, is licensed under a [FreeBSD License](https://github.com/jmcnamara/libxlsxwriter/blob/main/License.txt).

[3] [qt-collapsible-section](https://github.com/MichaelVoelkel/qt-collapsible-section), the code `ikoOSKAR::UI::ExamWidget` based on, is licensed under the [GNU Lesser General Public License version 3](https://github.com/MichaelVoelkel/qt-collapsible-section/blob/master/LICENSE).
