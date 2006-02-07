/*
 * song_editor.h - declaration of class songEditor, a window where you can
 *                 setup your songs
 *
 * Copyright (c) 2004-2006 Tobias Doerffel <tobydox/at/users.sourceforge.net>
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */


#ifndef _SONG_EDITOR_H
#define _SONG_EDITOR_H

#include "track_container.h"
#include "types.h"
#include "tool_button.h"


class QLabel;
class QScrollBar;
class QSlider;

class comboBox;
class lcdSpinBox;
class lmmsMainWin;
class pattern;
class projectNotes;
class textFloat;
class timeLine;


const int MIN_BPM = 10;
const int DEFAULT_BPM = 140;
const int MAX_BPM = 999;
const Uint16 MAX_SONG_LENGTH = 9999;


class songEditor : public trackContainer
{
	Q_OBJECT
public:
	enum playModes
	{
		PLAY_SONG,
		PLAY_TRACK,
		PLAY_BB,
		PLAY_PATTERN,
		PLAY_MODE_CNT
	} ;


	class playPos : public midiTime
	{
	public:
		playPos( Sint32 _abs = 0 ) :
			midiTime( _abs ),
			m_timeLine( NULL ),
			m_timeLineUpdate( TRUE ),
			m_currentFrame( 0 )
		{
		}
		inline void setCurrentFrame( Uint32 _f )
		{
			m_currentFrame = _f;
		}
		inline Uint32 currentFrame( void ) const
		{
			return( m_currentFrame );
		}
		timeLine * m_timeLine;
		bool m_timeLineUpdate;

	private:
		Uint32 m_currentFrame;
	} ;



	static inline songEditor * inst( void )
	{
		if( s_instanceOfMe == NULL )
		{
			s_instanceOfMe = new songEditor();
		}
		return( s_instanceOfMe );
	}

	void processNextBuffer( void );


	inline bool paused( void ) const
	{
		return( m_paused );
	}

	inline bool playing( void ) const
	{
		return( m_playing && m_exporting == FALSE );
	}

	inline bool exporting( void ) const
	{
		return( m_exporting );
	}

	bool realTimeTask( void ) const;

	inline bool exportDone( void ) const
	{
		return( m_exporting == TRUE &&
			m_playPos[PLAY_SONG].getTact() >= lengthInTacts() + 1 );
	}

	inline playModes playMode( void ) const
	{
		return( m_playMode );
	}

	inline playPos & getPlayPos( playModes _pm )
	{
		return( m_playPos[_pm] );
	}

	tact lengthInTacts( void ) const;


	int getBPM( void );

	// every function that replaces current file (e.g. creates new file,
	// opens another file...) has to call this before and may only process
	// if this function returns true
	bool mayChangeProject( void );


	float framesPerTact( void ) const;

	// file management
	void createNewProject( void );
	void FASTCALL createNewProjectFromTemplate( const QString & _template );
	void FASTCALL loadProject( const QString & _file_name );
	bool saveProject( void );
	bool FASTCALL saveProjectAs( const QString & _file_name );
	inline const QString & projectFileName( void ) const
	{
		return( m_fileName );
	}
	virtual void FASTCALL saveSettings( QDomDocument & _doc,
							QDomElement & _parent );
	virtual void FASTCALL loadSettings( const QDomElement & _this );
	inline virtual QString nodeName( void ) const
	{
		return( "songeditor" );
	}

	virtual inline bool fixedTCOs( void ) const
	{
		return( FALSE );
	}

	int masterPitch( void ) const;

	projectNotes * getProjectNotesWindow( void )
	{
		return( m_projectNotes );
	}


public slots:
	void play( void );
	void stop( void );
	void playTrack( track * _trackToPlay );
	void playBB( void );
	void playPattern( pattern * _patternToPlay, bool _loop = TRUE );
	void pause( void );
	void resumeFromPause( void );

	void importProject( void );
	void exportProject( void );

	void startExport( void );
	void stopExport( void );
	// set BPM (beats per minute)
	void setBPM( int _new_bpm = DEFAULT_BPM );

	inline void setModified( void )
	{
		m_modified = TRUE;
	}


protected:
	virtual void closeEvent( QCloseEvent * _ce );
	virtual void resizeEvent( QResizeEvent * _re );
	virtual void keyPressEvent( QKeyEvent * _ke );
	virtual void wheelEvent( QWheelEvent * _we );
	virtual void paintEvent( QPaintEvent * _pe );

	virtual QRect scrollAreaRect( void ) const;

	virtual bool allowRubberband( void ) const
	{
		return( m_editModeButton->isChecked() );
	}


protected slots:
	void insertBar( void );
	void removeBar( void );
	void addBBTrack( void );
	void addSampleTrack( void );
	void scrolled( int _new_pos );
	void updateTimeLinePosition( void );

	void masterVolumeChanged( int _new_val );
	void masterVolumePressed( void );
	void masterVolumeMoved( int _new_val );
	void masterVolumeReleased( void );
	void masterPitchChanged( int _new_val );
	void masterPitchPressed( void );
	void masterPitchMoved( int _new_val );
	void masterPitchReleased( void );

	void updatePosition( const midiTime & _t );

	void zoomingChanged( const QString & _zfac );

	void doActions( void );


private:
	songEditor();
	songEditor( const songEditor & );
	virtual ~songEditor();

	void clearProject( void );


	inline tact currentTact( void ) const
	{
		return( m_playPos[m_playMode].getTact() );
	}

	midiTime length( void ) const;
	inline tact64th currentTact64th( void ) const
	{
		return( m_playPos[m_playMode].getTact64th() );
	}
	void FASTCALL setPlayPos( tact _tact_num, tact64th _t_64th, playModes
								_play_mode );



	static songEditor * s_instanceOfMe;

	QScrollBar * m_leftRightScroll;

	QWidget * m_toolBar;

	toolButton * m_playButton;
	toolButton * m_stopButton;
	lcdSpinBox * m_bpmSpinBox;

	QSlider * m_masterVolumeSlider;
	QSlider * m_masterPitchSlider;
	textFloat * m_mvsStatus;
	textFloat * m_mpsStatus;

	toolButton * m_addBBTrackButton;
	toolButton * m_addSampleTrackButton;

	toolButton * m_drawModeButton;
	toolButton * m_editModeButton;

	comboBox * m_zoomingComboBox;


	QString m_fileName;
	QString m_oldFileName;
	bool m_modified;

	volatile bool m_exporting;
	volatile bool m_playing;
	volatile bool m_paused;

	bool m_loadingProject;

	playModes m_playMode;
	playPos m_playPos[PLAY_MODE_CNT];

	track * m_trackToPlay;
	pattern * m_patternToPlay;
	bool m_loopPattern;

	bool m_scrollBack;


	projectNotes * m_projectNotes;



	enum ACTIONS
	{
		ACT_STOP_PLAY, ACT_PLAY_SONG, ACT_PLAY_TRACK, ACT_PLAY_BB,
		ACT_PLAY_PATTERN, ACT_PAUSE, ACT_RESUME_FROM_PAUSE
	} ;
	vvector<ACTIONS> m_actions;


	friend class lmmsMainWin;



signals:
	void bpmChanged( int _new_bpm );

} ;


#endif
