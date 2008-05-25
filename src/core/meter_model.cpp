/*
 * meter_model.cpp - model for meter specification
 *
 * Copyright (c) 2008 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 * 
 * This file is part of Linux MultiMedia Studio - http://lmms.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#include "meter_model.h"


meterModel::meterModel( ::model * _parent, track * _track ) :
	model( _parent ),
	m_numeratorModel( 4, 1, 32, 1, this ),
	m_denominatorModel( 4, 1, 32, 1, this )
{
	m_numeratorModel.setTrack( _track );
	m_denominatorModel.setTrack( _track );

	connect( &m_numeratorModel, SIGNAL( dataChanged() ), 
				this, SIGNAL( dataChanged() ) );
	connect( &m_denominatorModel, SIGNAL( dataChanged() ), 
				this, SIGNAL( dataChanged() ) );
}




meterModel::~meterModel()
{
}




void meterModel::reset( void )
{
	m_numeratorModel.setValue( 4 );
	m_denominatorModel.setValue( 4 );
}




void meterModel::saveSettings( QDomDocument & _doc, QDomElement & _this,
							const QString & _name )
{
	m_numeratorModel.saveSettings( _doc, _this, _name + "_numerator" );
	m_denominatorModel.saveSettings( _doc, _this, _name + "_denominator" );
}




void meterModel::loadSettings( const QDomElement & _this,
							const QString & _name )
{
	m_numeratorModel.loadSettings( _this, _name + "_numerator" );
	m_denominatorModel.loadSettings( _this, _name + "_denominator" );
}



#include "meter_model.moc"

