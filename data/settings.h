/* Settings is used to load and save the settings of
 * the software after closure.
 *
 * More options could be added later on, only the
 * general geometry of the main window is currently
 * implemented.
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QPoint>
#include <QSize>

class Settings {
public:
    Settings();
    void loadSettings();
    void saveSettings();
    QPoint getWindowPosition() const;
    QSize getWindowSize() const;
    void setWindowPosition(const QPoint &position);
    void setWindowSize(const QSize &size);

private:
    QSettings settings;
    QPoint windowPosition;
    QSize windowSize;
};

#endif // SETTINGS_H
