//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_DATABASE_I_DATABASE_RESULT_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_RESULT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DatabaseConnection;
class I_DatabaseResult;
class I_DatabaseRow;
class I_DatabaseColumn;

class DATABASE_DLL_LINK I_DatabaseResult
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseResult>          pDatabaseResult_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseResult>     wpDatabaseResult_type;
    typedef Zen::Event::Event<wpDatabaseResult_type>            databaseResultEvent_type;

    struct I_ResultVisitor;
    struct I_ColumnVisitor;
    /// @}

    /// @name I_DatabaseResult interface
    /// @{
public:
    virtual void getResults(I_ResultVisitor& _pVisitor) = 0;

    virtual void getColumns(I_ColumnVisitor& _pVisitor) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    databaseResultEvent_type    onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseResult();
    virtual ~I_DatabaseResult();
    /// @}

    /// @name Inner Clases
    /// @{
public:
    /// @todo Rename this to I_RowVisitor?  TR - I like that name better.
    struct I_ResultVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_DatabaseRow& _databaseRow) = 0;
        virtual void end() = 0;
    };  // interface I_ResultVisitor

    struct I_ColumnVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_DatabaseColumn& _databaseColumn) = 0;
        virtual void end() = 0;
    };  // interface I_ColumnVisitor
    /// @}

};  // interface I_DatabaseResult

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory {
    // I_DatabaseResult is managed by I_DatabaseTransaction
    template<>
    struct is_managed_by_factory<Database::I_DatabaseResult> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_RESULT_HPP_INCLUDED
