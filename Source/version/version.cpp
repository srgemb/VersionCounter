
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <ctime>

#include "ini.h"
#include "INIReader.h"

//*************************************************************************************************
// Тип сборки
//*************************************************************************************************
typedef enum {
    MODE_DEBUG,
    MODE_RELASE
} TypeMode;

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
//TypeMode mode;
tm *loctm;
char path[256], modes[20];
ofstream ver_file, ini_file, log_file; // поток для записи
string source, header, def, d_version, p_version, r_version, sdtime;
string delim = "--------------------------------------------------------------------------------------";
//значения номера версии для DEBUG
int d_major, d_minor, d_build;
//значения номера версии для PRERELEASE
int p_major, p_minor, p_build;
//значения номера версии для RELEASE
int r_major, r_minor, r_build;
//значения дата/время для версии DEBUG
int d_hour, d_min, d_sec, d_day, d_mon, d_year;
//значения дата/время для версии PRERELEASE
int p_hour, p_min, p_sec, p_day, p_mon, p_year;
//значения дата/время для версии RELEASE
int r_hour, r_min, r_sec, r_day, r_mon, r_year;

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
bool ReadIni( string fname );
bool SaveIni( string fname );
bool CrtHeader( string hdr );

//*************************************************************************************************
// MAIN
//*************************************************************************************************
int main( int argc, char *argv[] ) {

    if ( argc != 3 ) {
        cout << endl << endl;
        cout << "Usage: version.exe debug/pre_release/release full path to the version.ini file." << endl;
        cout << "For example: version.exe debug C:\\Project\\" << endl;
        cout << "For example: version.exe release C:\\Project\\" << endl;
        cout << "Each time it starts, the value of the \"build\" parameter in the version.ini" << endl;
        cout << "file is increased by 1. The cycle is 0-255." << endl;
        cout << "Product version consists of X.Y.Z (major.minor.build)." << endl;
        cout << "X - when fully updated code comes out." << endl;
        cout << "Y - when changing, adding new functionality." << endl;
        cout << "Z - increases automatically when the project is rebuilt (when fixing bugs)." << endl << endl;
        system( "pause" );
        return 0;
    }

    //текущая дата/время
    time_t now = time( NULL );
    loctm = localtime( &now );
    loctm->tm_year += 1900; //years since 1900
    loctm->tm_mon++;        //months since January – [0, 11]

    //вывод пути запуска
    cout << "Start: " << argv[0] << endl;

    //чтение параметра сборки
    memset( modes, 0x00, sizeof( modes ) );
    memcpy( modes, argv[1], strlen( argv[1] ) );
    //имя секции из INI файла
    for ( int i = 0; i < strlen( modes ); i++ )
        modes[i] = toupper( modes[i] );

    if ( strcmp( modes, "DEBUG" ) != 0 && strcmp( modes, "RELEASE" ) != 0 && strcmp( modes, "PRE_RELEASE" ) != 0 )
        return 0;

    /*mode = MODE_RELASE;
else mode = MODE_DEBUG;*/

//чтение параметра пути к INI файла
    memset( path, 0x00, sizeof( path ) );
    memcpy( path, argv[2], strlen( argv[2] ) );
    if ( path[strlen( path ) - 1] != '\\' )
        strcat( path, "\\" );
    strcat( path, "version.ini" );
    //разделитель
    cout << delim << endl;

    //чтение параметров из INI файла
    cout << "Load file: " << path << endl;
    if ( ReadIni( path ) == false ) {
        cout << "Can't load: " << path << endl;
        cout << delim << endl;
        return 1;
    }
    //изменяем значения
    if ( strcmp( modes, "DEBUG" ) == 0 ) {
        d_build++;
        if ( d_build > 255 )
            d_build = 0;
        cout << "Major     = " << d_major << endl;
        cout << "Minor     = " << d_minor << endl;
        cout << "Build     = " << d_build << endl;
        cout << "Version   = " << d_version << endl;
        cout << "Date/time = ";
        cout << setw( 2 ) << setfill( '0' ) << d_day << ".";
        cout << setw( 2 ) << setfill( '0' ) << d_mon << ".";
        cout << setw( 4 ) << setfill( '0' ) << d_year << "  ";
        cout << setw( 2 ) << setfill( '0' ) << d_hour << ":";
        cout << setw( 2 ) << setfill( '0' ) << d_min << ":";
        cout << setw( 2 ) << setfill( '0' ) << d_sec << endl;
    }
    if ( strcmp( modes, "PRE_RELEASE" ) == 0 ) {
        p_build++;
        if ( p_build > 255 )
            p_build = 0;
        cout << "Major     = " << p_major << endl;
        cout << "Minor     = " << p_minor << endl;
        cout << "Build     = " << p_build << endl;
        cout << "Version   = " << p_version << endl;
        cout << "Date/time = ";
        cout << setw( 2 ) << setfill( '0' ) << p_day << ".";
        cout << setw( 2 ) << setfill( '0' ) << p_mon << ".";
        cout << setw( 4 ) << setfill( '0' ) << p_year << "  ";
        cout << setw( 2 ) << setfill( '0' ) << p_hour << ":";
        cout << setw( 2 ) << setfill( '0' ) << p_min << ":";
        cout << setw( 2 ) << setfill( '0' ) << p_sec << endl;
    }
    if ( strcmp( modes, "RELEASE" ) == 0 ) {
        r_build++;
        if ( r_build > 255 )
            r_build = 0;
        cout << "Major     = " << r_major << endl;
        cout << "Minor     = " << r_minor << endl;
        cout << "Build     = " << r_build << endl;
        cout << "Version   = " << r_version << endl;
        cout << "Date/time = ";
        cout << setw( 2 ) << setfill( '0' ) << r_day << ".";
        cout << setw( 2 ) << setfill( '0' ) << r_mon << ".";
        cout << setw( 4 ) << setfill( '0' ) << r_year << "  ";
        cout << setw( 2 ) << setfill( '0' ) << r_hour << ":";
        cout << setw( 2 ) << setfill( '0' ) << r_min << ":";
        cout << setw( 2 ) << setfill( '0' ) << r_sec << endl;
    }

    //сохраняем новые значения в INI файле
    if ( SaveIni( path ) == false ) {
        cout << "Error save file: " << path << endl;
        cout << delim << endl;
        return 2;
    }

    //формируем файл заголовка
    cout << "Save header file: " << source + header << endl;
    if ( CrtHeader( source + header ) == false ) {
        cout << "Error save file: " << source + header << endl;
        cout << delim << endl;
        return 3;
    }
    cout << delim << endl;

    //формирование LOG файла
    memset( path, 0x00, sizeof( path ) );
    memcpy( path, argv[2], strlen( argv[2] ) );
    if ( path[strlen( path )-1] != '\\' )
        strcat( path, "\\" );
    strcat( path, "version.log" );
    //открываем файл
    log_file.open( path, ios::app );
    if ( !log_file.is_open( ) )
        return false;

    //запись дата/время
    log_file << setw( 2 ) << setfill( '0' ) << loctm->tm_mday << ".";
    log_file << setw( 2 ) << setfill( '0' ) << loctm->tm_mon << ".";
    log_file << setw( 4 ) << setfill( '0' ) << loctm->tm_year << " ";
    log_file << setw( 2 ) << setfill( '0' ) << loctm->tm_hour << ":";
    log_file << setw( 2 ) << setfill( '0' ) << loctm->tm_min << ":";
    log_file << setw( 2 ) << setfill( '0' ) << loctm->tm_sec << " - ";

    //запись в LOG файл номера версии
    if ( strcmp( modes, "DEBUG" ) == 0 ) {
        log_file << d_major << ".";
        log_file << d_minor << ".";
        log_file << d_build << ".";
        log_file << d_version << endl;
    }
    if ( strcmp( modes, "PRE_RELEASE" ) == 0 ) {
        log_file << p_major << ".";
        log_file << p_minor << ".";
        log_file << p_build << ".";
        log_file << p_version << endl;
    }
    if ( strcmp( modes, "RELEASE" ) == 0 ) {
        log_file << r_major << ".";
        log_file << r_minor << ".";
        log_file << r_build << ".";
        log_file << r_version << endl;
    }
    log_file.close( );

    return 0;
}

//*************************************************************************************************
// Чтение параметров из INI файла
// string fname - полный путь и имя INI файла для чтения
//*************************************************************************************************
bool ReadIni( string fname ) {

    INIReader reader( fname );
    if ( reader.ParseError( ) < 0 )
        return false;
    //общие параметры
    source = reader.GetString( "main", "source", "" );  //куда формируем H файла
    header = reader.GetString( "main", "header", "" );  //имя H файла
    //def = reader.GetString("main", "define", "" );     //имя параметра для define

    //номер версии DEBUG
    d_version = reader.GetString("debug", "version", "D" );
    d_major = reader.GetInteger( "debug", "major", 0 );
    d_minor = reader.GetInteger( "debug", "minor", 0 );
    d_build = reader.GetInteger( "debug", "build", 0 );
    //время
    d_hour = reader.GetInteger( "debug", "hour", 0 );
    d_min = reader.GetInteger( "debug", "minutes", 0 );
    d_sec = reader.GetInteger( "debug", "seconds", 0 );
    //дата
    d_day = reader.GetInteger( "debug", "day", 0 );
    d_mon = reader.GetInteger( "debug", "month", 0 );
    d_year = reader.GetInteger( "debug", "year", 0 );

    //номер версии PRE_RELEASE
    p_version = reader.GetString("pre_release", "version", "P" );
    p_major = reader.GetInteger( "pre_release", "major", 0 );
    p_minor = reader.GetInteger( "pre_release", "minor", 0 );
    p_build = reader.GetInteger( "pre_release", "build", 0 );
    //время
    p_hour = reader.GetInteger( "pre_release", "hour", 0 );
    p_min = reader.GetInteger( "pre_release", "minutes", 0 );
    p_sec = reader.GetInteger( "pre_release", "seconds", 0 );
    //дата
    p_day = reader.GetInteger( "pre_release", "day", 0 );
    p_mon = reader.GetInteger( "pre_release", "month", 0 );
    p_year = reader.GetInteger( "pre_release", "year", 0 );

    //номер версии RELEASE
    r_version = reader.GetString("release", "version", "R" );
    r_major = reader.GetInteger( "release", "major", 0 );
    r_minor = reader.GetInteger( "release", "minor", 0 );
    r_build = reader.GetInteger( "release", "build", 0 );
    //время
    r_hour = reader.GetInteger( "release", "hour", 0 );
    r_min = reader.GetInteger( "release", "minutes", 0 );
    r_sec = reader.GetInteger( "release", "seconds", 0 );
    //дата
    r_day = reader.GetInteger( "release", "day", 0 );
    r_mon = reader.GetInteger( "release", "month", 0 );
    r_year = reader.GetInteger( "release", "year", 0 );
    return true;
}

//*************************************************************************************************
// Сохранение параметров
// string fname - полный путь и имя INI файла для сохранения
//*************************************************************************************************
bool SaveIni( string fname ) {

    ini_file.open( fname ); // окрываем файл для записи
    if ( !ini_file.is_open( ) )
        return false;
    ini_file << "[main]" << endl;
    ini_file << "source = " << source << endl;
    ini_file << "header = " << header << endl;
    //ini_file << "define = " << def << endl;

    //для версии DEBUG
    ini_file << endl;
    ini_file << "[debug]" << endl;
    ini_file << "version = " << d_version << endl;
    ini_file << "major   = " << d_major << endl;
    ini_file << "minor   = " << d_minor << endl;
    ini_file << "build   = " << d_build << endl << endl;
    //время
    if ( strcmp( modes, "DEBUG" ) == 0 ) {
        //присваиваем новые значения дата/время
        d_hour = loctm->tm_hour;
        d_min = loctm->tm_min;
        d_sec = loctm->tm_sec;
        d_day = loctm->tm_mday;
        d_mon = loctm->tm_mon;
        d_year = loctm->tm_year;
    }
    //сохраним новые значения дата/время
    ini_file << "hour    = " << d_hour << endl;
    ini_file << "minutes = " << d_min << endl;
    ini_file << "seconds = " << d_sec << endl << endl;
    ini_file << "day     = " << d_day << endl;
    ini_file << "month   = " << d_mon << endl;
    ini_file << "year    = " << d_year << endl;

    //для версии PRERELEASE
    ini_file << endl;
    ini_file << "[pre_release]" << endl;
    ini_file << "version = " << p_version << endl;
    ini_file << "major   = " << p_major << endl;
    ini_file << "minor   = " << p_minor << endl;
    ini_file << "build   = " << p_build << endl << endl;
    if ( strcmp( modes, "PRE_RELEASE" ) == 0 ) {
        //присвоим новые значения дата/время
        p_hour = loctm->tm_hour;
        p_min = loctm->tm_min;
        p_sec = loctm->tm_sec;
        p_day = loctm->tm_mday;
        p_mon = loctm->tm_mon;
        p_year = loctm->tm_year;
    }
    //сохраним новые значения дата/время
    ini_file << "hour    = " << p_hour << endl;
    ini_file << "minutes = " << p_min << endl;
    ini_file << "seconds = " << p_sec << endl << endl;
    ini_file << "day     = " << p_day << endl;
    ini_file << "month   = " << p_mon << endl;
    ini_file << "year    = " << p_year << endl;

    ini_file << endl;
    ini_file << "[release]" << endl;
    ini_file << "version = " << r_version << endl;
    ini_file << "major   = " << r_major << endl;
    ini_file << "minor   = " << r_minor << endl;
    ini_file << "build   = " << r_build << endl << endl;
    if ( strcmp( modes, "RELEASE" ) == 0 ) {
        //присвоим новые значения дата/время
        r_hour = loctm->tm_hour;
        r_min = loctm->tm_min;
        r_sec = loctm->tm_sec;
        r_day = loctm->tm_mday;
        r_mon = loctm->tm_mon;
        r_year = loctm->tm_year;
    }
    //пишем старые значения (из INI файла)
    ini_file << "hour    = " << r_hour << endl;
    ini_file << "minutes = " << r_min << endl;
    ini_file << "seconds = " << r_sec << endl << endl;
    ini_file << "day     = " << r_day << endl;
    ini_file << "month   = " << r_mon << endl;
    ini_file << "year    = " << r_year << endl;
    return true;
}

//*************************************************************************************************
// Cформируем H файл
//*************************************************************************************************
bool CrtHeader( string hdr ) {

    ver_file.open( hdr );
    if ( !ver_file.is_open( ) )
        return false;
    ver_file << endl;
    ver_file << "#ifndef __VERDATA_H" << endl;
    ver_file << "#define __VERDATA_H" << endl << endl;

    //DEBUG
    ver_file << "#ifdef " << "FW_DEBUG" << endl;
    ver_file << "    #define FW_VERSION_MAJOR   " << d_major << endl;
    ver_file << "    #define FW_VERSION_MINOR   " << d_minor << endl;
    ver_file << "    #define FW_VERSION_BUILD   " << d_build << endl;
    ver_file << "    #define FW_VERSION_RC      " << "'" << d_version << "'" << endl << endl;

    ver_file << "    #define FW_TIME_HOUR       " << d_hour << endl;
    ver_file << "    #define FW_TIME_MINUTES    " << d_min << endl;
    ver_file << "    #define FW_TIME_SECONDS    " << d_sec << endl << endl;
    ver_file << "    #define FW_DATE_DAY        " << d_day << endl;
    ver_file << "    #define FW_DATE_MONTH      " << d_mon << endl;
    ver_file << "    #define FW_DATE_YEAR       " << d_year << endl;
    ver_file << "#endif" << endl << endl;
    
    //PRE_RELEASE
    ver_file << "#ifdef " << "FW_PRE_RELEASE" << endl;
    ver_file << "    #define FW_VERSION_MAJOR   " << p_major << endl;
    ver_file << "    #define FW_VERSION_MINOR   " << p_minor << endl;
    ver_file << "    #define FW_VERSION_BUILD   " << p_build << endl;
    ver_file << "    #define FW_VERSION_RC      " << "'" << p_version << "'" << endl << endl;

    ver_file << "    #define FW_TIME_HOUR       " << p_hour << endl;
    ver_file << "    #define FW_TIME_MINUTES    " << p_min << endl;
    ver_file << "    #define FW_TIME_SECONDS    " << p_sec << endl << endl;
    ver_file << "    #define FW_DATE_DAY        " << p_day << endl;
    ver_file << "    #define FW_DATE_MONTH      " << p_mon << endl;
    ver_file << "    #define FW_DATE_YEAR       " << p_year << endl;
    ver_file << "#endif" << endl << endl;

    //RELEASE
    ver_file << "#ifdef " << "FW_RELEASE" << endl;
    ver_file << "    #define FW_VERSION_MAJOR   " << r_major << endl;
    ver_file << "    #define FW_VERSION_MINOR   " << r_minor << endl;
    ver_file << "    #define FW_VERSION_BUILD   " << r_build << endl;
    ver_file << "    #define FW_VERSION_RC      " << "'" << r_version << "'" << endl << endl;

    ver_file << "    #define FW_TIME_HOUR       " << r_hour << endl;
    ver_file << "    #define FW_TIME_MINUTES    " << r_min << endl;
    ver_file << "    #define FW_TIME_SECONDS    " << r_sec << endl << endl;
    ver_file << "    #define FW_DATE_DAY        " << r_day << endl;
    ver_file << "    #define FW_DATE_MONTH      " << r_mon << endl;
    ver_file << "    #define FW_DATE_YEAR       " << r_year << endl;
    ver_file << "#endif" << endl << endl;

    ver_file << "#endif" << endl << endl;
    ver_file.close( );
    return true;
}
