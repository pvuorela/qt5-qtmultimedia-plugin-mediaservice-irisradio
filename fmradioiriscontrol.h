/*
  Copyright (C) 2016 Matti Lehtimäki
  Contact: Matti Lehtimäki <matti.lehtimaki@gmail.com>
  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __FMRADIOIRISCONTROL_H
#define __FMRADIOIRISCONTROL_H

#include <QDateTime>
#include <QRadioTunerControl>
#include <QRadioTuner>
#include <QRadioDataControl>
#include <QRadioData>
#include <QTimer>
#include <pthread.h>

QT_BEGIN_NAMESPACE

class FMRadioIrisControl : public QObject
{
    Q_OBJECT
public:
    FMRadioIrisControl();
    ~FMRadioIrisControl();

    bool isTunerAvailable() const;
    QMultimedia::AvailabilityStatus tunerAvailability() const;

    QRadioTuner::State tunerState() const;

    QRadioTuner::Band band() const;
    void setBand(QRadioTuner::Band b);
    bool isBandSupported(QRadioTuner::Band b) const;

    int frequency() const;
    void setFrequency(int frequency);

    bool isStereo() const;
    QRadioTuner::StereoMode stereoMode() const;
    void setStereoMode(QRadioTuner::StereoMode mode);

    int signalStrength() const;

    int volume() const;
    void setVolume(int volume);

    bool isMuted() const;
    void setMuted(bool m_muted);

    void setSearching(bool search);
    bool isSearching() const;

    bool isAntennaConnected() const;

    void searchForward();
    void searchBackward();
    void searchAllStations(QRadioTuner::SearchMode m_searchMode = QRadioTuner::SearchFast);
    void cancelSearch();

    void start();
    void stop();

    QRadioTuner::Error tunerError() const;
    QString tunerErrorString() const;

    bool isRdsAvailable() const;
    QMultimedia::AvailabilityStatus rdsAvailability() const;

    QString stationId() const;
    QRadioData::ProgramType programType() const;
    QString programTypeName() const;
    QString stationName() const;
    QString radioText() const;
    void setAlternativeFrequenciesEnabled(bool enabled);
    bool isAlternativeFrequenciesEnabled() const;

    QRadioData::Error rdsError() const;
    QString rdsErrorString() const;

signals:
    void tunerAvailabilityChanged(bool available);
    void tunerAvailabilityChanged(QMultimedia::AvailabilityStatus availability);
    void stateChanged(QRadioTuner::State state);
    void bandChanged(QRadioTuner::Band band);
    void frequencyChanged(int frequency);
    void stereoStatusChanged(bool m_stereo);
    void searchingChanged(bool searching);
    void signalStrengthChanged(int signalStrength);
    void volumeChanged(int volume);
    void mutedChanged(bool m_muted);
    void error(QRadioTuner::Error err);
    void stationFound(int frequency, QString stationId);
    void antennaConnectedChanged(bool connectionStatus);

    void stationIdChanged(QString stationId);
    void programTypeChanged(QRadioData::ProgramType programType);
    void programTypeNameChanged(QString programTypeName);
    void stationNameChanged(QString stationName);
    void radioTextChanged(QString radioText);
    void alternativeFrequenciesEnabledChanged(bool enabled);
    void error(QRadioData::Error err);

private slots:
    void search();

private:
    pthread_t m_eventListenerThread;

    int m_fd;

    bool m_tunerError;
    bool m_muted;
    bool m_stereo;
    bool m_low;
    bool m_tunerAvailable;
    int  m_step;
    int  m_signalStrength;
    bool m_scanning;
    bool m_forward;
    QRadioTuner::Band m_currentBand;
    qint64 m_freqMin;
    qint64 m_freqMax;
    qint64 m_currentFreq;
    QTime  m_playTime;
    QTimer* m_timer;
    QRadioTuner::SearchMode m_searchMode;
    qint64 m_searchStartFreq;
    qint64 m_searchPreviousFreq;

    bool m_rdsAvailable;
    bool m_rdsError;
    QString m_stationId;
    QRadioData::ProgramType m_programType;
    QString m_programTypeName;
    QString m_stationName;
    QString m_radioText;
    bool m_alternativeFrequenciesEnabled;

    static void *EventListener(void* context);

    bool initRadio();
    void doSeek(int dir);
    bool SetFreq(int frequency);//Hz
    int GetFreq(void);//Hz
    void GetCaps(void);
    int GetBuffer(int type);
    int GetEvent();
    void DoSeek(int dir);//0 is down, 1 is up
    bool SetTuner();
    bool GetTuner();
    bool SetCtrl(int id, int value);
    int GetCtrl(int id);
    unsigned int programTypeValue(int rdsStandard, unsigned int type);
    QString programTypeNameString(int rdsStandard, unsigned int type);
};

QT_END_NAMESPACE

#endif // __FMRADIOIRISCONTROL_H
