#ifndef TIME_HELPER_H
#define TIME_HELPER_H

#include <QDateTime>
#include <QTimeZone>

inline QDateTime seconds_to_datetime(float seconds) {
	return QDateTime::fromMSecsSinceEpoch(seconds*1000, QTimeZone::utc());
}

#endif // TIME_HELPER_H
