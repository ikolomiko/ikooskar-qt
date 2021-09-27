# What is ikoOSKAR
ikoOSKAR (iko Ortak Sınav Karma Sistemi) allows you to easily manage
your school's student placement schemes for exams. This program is especially
targeted for high schools.

With ikoOSKAR you can:

* Create student placement schemes with minimum same-grade student interaction
* Easily manage the student database
* // TODO

# ikoOSKAR-Qt
This project is a reimplementation of ikoOSKAR which was originally written in 
C# and .Net Framework

I used C++11 and Qt Framework 5.13 for this reimplementation, which makes the 
program cross-platform
and also more efficient

## Dependencies for development
* Qt Creator (optional but highly recommended)
* Qt Framework >= 5.13
* Compiler: GCC (for Linux), MinGW (for Windows)
  - I use [a script](/compile4win32.sh) that uses MXE (with 
  `i686-w64-mingw32.static-qmake-qt5` package) for cross-compiling to Windows
  - You won't need this if your host OS is Windows

## Dependencies for the end user
* Any program that can handle .xlsx files (e.g. MS Office, LibreOffice, WPS Office)
* Internet connection for the first activation

## Documentation
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
You can see which class belongs to which layer in the tables below


DAL				|	BLL				|	UI
----------------+-------------------+----------------------
Database 		| DatabaseHelper 	| DatabaseUi
MultiImport		| MultiImportHelper	| MultiImportUi
- 				| StudentEditor		| StudentEditorUi
- 				| SchemeGenerator	| SchemeGeneratorUi
ExcelWriter		| SchemeExporter	| -
HistoryReader 	| HistoryHelper 	| HistoryUi
- 				| AboutHelper 		| AboutUi
- 				| - 				| MainPage
LocalAuth 		| LocalAuthHelper 	| -
CloudAuth 		| CloudAuthHelper 	| -
- 				| Authenticator 	| AuthenticatorPage
- 				| - 				| SplashPage


|	Shared 		|
|:-------------:|
|Student		|
|Desk			|
|Classroom		|
|ExamStudent	|
