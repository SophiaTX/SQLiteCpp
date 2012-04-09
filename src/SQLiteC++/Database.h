/**
 * @file  Database.h
 * @brief Management of a SQLite Database Connection.
 *
 * Copyright (c) 2012 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sqlite3.h>
#include "Exception.h"

namespace SQLite
{

// Forward declarations
class Statement;
class Exception;

/**
 * @brief RAII management of a SQLite Database Connection.
 *
 * A Database object manage a list of all SQLite Statements associated with the
 * underlying SQLite 3 database connection.
 *
 * Resource Acquisition Is Initialization (RAII) means that the Database Connection
 * is opened in the constructor and closed in the destructor, so that there is
 * no need to worry about memory management or the validity of the underlying SQLite Connection.
 */
class Database
{
    friend class Statement;

public:
    /**
     * @brief Open the provided database UTF-8 filename.
     *
     * Uses sqlite3_open_v2() with readonly default flag, which is the opposite behavior
     * of the old sqlite3_open() function (READWRITE+CREATE).
     * This makes sense if you want to use it on a readonly filesystem
     * or to prevent creation of a void file when a required file is missing.
     *
     * Exception is thrown in case of error, then the Database object is NOT constructed.
     *
     * @param[in] apFilename    UTF-8 path/uri to the database file ("filename" sqlite3 parameter)
     * @param[in] aFlags        SQLITE_OPEN_READONLY/SQLITE_OPEN_READWRITE/SQLITE_OPEN_CREATE...
     */
    explicit Database(const char* apFilename, const int aFlags = SQLITE_OPEN_READONLY); // throw(SQLite::Exception);

    /**
     * @brief Close the SQLite database connection.
     *
     * All SQLite statements must have been finalized before,
     * so all Statement objects must have been unregistered.
     */
    virtual ~Database(void) throw(); // nothrow

    /**
     * @brief Shortcut to execute one or multiple statements without results.
     *
     *  This is useful for Data Manipulation Language SQL statements like CREATE, INSERT, UPDATE, DROP
     *
     * @see also Statement class for handling queries with results
     *
     * @param[in] apQueries  one or multiple UTF-8 encoded, semicolon-separate SQL statements
     */
    int exec(const char* apQueries); // throw(SQLite::Exception);

    /**
     * @brief Filename used to open the database
     */
    inline const std::string& getFilename(void) const
    {
        return mFilename;
    }

private:
    // Database must not be copyable
    Database(void);
    Database(const Database&);
    Database& operator=(const Database&);

    /**
     * @brief Check if aRet equal SQLITE_OK, else throw a SQLite::Exception with the SQLite error message
     */
    void check(const int aRet) const; // throw(SQLite::Exception);

private:
    sqlite3*    mpSQLite;   //!< Pointer to SQLite Database Connection Handle
    std::string mFilename;  //!< UTF-8 filename used to open the database
};


};  // namespace SQLite
