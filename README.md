# What is ikoOSKAR
ikoOSKAR (iko Ortak Sınav Karma Sistemi) allows you to easily manage
your school's student placement schemes for exams. This program is especially
targeted for high schools.

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

# ikoOSKAR-Qt
This project is a reimplementation of ikoOSKAR which was originally written in 
C# and .Net Framework.

I use C++11 and Qt Framework 5.x for this reimplementation, which makes the 
program cross-platform and also more efficient.

The code formatter of my choice is `clang-format` with `-style=webkit` styling 
option.

## Dependencies for development
* Qt Creator (optional but highly recommended)
* Qt Framework >= 5.13
* Compiler: GCC (for Linux), MinGW (for Windows)
  - I use [a script](/compile4win32.sh) that uses MXE (with 
  `i686-w64-mingw32.static-qmake-qt5` package) for cross-compiling to Windows. 
  You won't need this if your host OS is Windows
* Code formatter (optional): `clang-format`

## Dependencies for the end user
* Any program that can handle .xlsx files (e.g. MS Office, LibreOffice, WPS 
Office)
* Internet connection for the first activation

# Documentation
This project consists of 4 layers:

1. Data Access Layer (DAL for short): This is the layer which interacts with raw
 data; such as the database, the filesystem and the CloudAuth API
2. Business Logic Layer (BLL for short): This is the layer which contains the 
program logic and interacts between the UI and the DAL
3. User Interface Layer (UI for short) (aka Presentation Layer): This is the 
layer which handles all UI operations
4. Shared Layer (aka Entities): This layer contains shared classes and 
information across all the other layers


&nbsp;

## Contents of Layers
You can see which class belongs to which layer in the tables below. Classes with 
direct relationship are represented at the same row. The Shared Layer is shown
separately because its classes are used commonly among other classes and do NOT
have a direct relationship with only one (or two) class(es).

You can click on the elements of the tables to navigate to corresponding 
file/folder.


|***[DAL](/DAL/)***|***[BLL](/BLL/)***	|	***[UI](/UI/)***
----------------|-------------------|-----------------------
| [Database](/DAL/Database/) | [DatabaseHelper](/BLL/DatabaseHelper/) | [DatabaseUi](UI/DatabaseUi/)
| MultiImport	| MultiImportHelper	| MultiImportUi
| -				| StudentEditor		| StudentEditorUi
| -				| SchemeGenerator	| SchemeGeneratorUi
| ExcelWriter	| SchemeExporter	| -
| HistoryReader	| HistoryHelper 	| HistoryUi
| -				| AboutHelper 		| AboutUi
| - 			| - 				| MainPage
| LocalAuth		| LocalAuthHelper 	| -
| CloudAuth		| CloudAuthHelper 	| -
| - 			| Authenticator 	| AuthenticatorPage
| -				| - 				| SplashPage
| -       		| -         		| [ErrorUi](/UI/ErrorUi)


|***[Shared](/Shared/)***	|
|:-------------:|
|[Student](/Shared/student.h)		|
|Desk			|
|Classroom		|
|ExamStudent	|
