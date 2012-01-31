/***********************************************************************
 *
 * This file is part of Anthaxia.
 * 
 * Anthaxia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Anthaxia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef _TEST_MARSHALLING_H
#define _TEST_MARSHALLING_H

#include <cppunit/extensions/HelperMacros.h>
#include <servicesystem/marshalling.h>

class MarshallingTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE( MarshallingTest );
    CPPUNIT_TEST( testIntMarshalling );
    CPPUNIT_TEST( testDoubleMarshalling );
    CPPUNIT_TEST( testStringMarshalling );
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testIntMarshalling();
    void testDoubleMarshalling();
    void testStringMarshalling();
};

#endif
