#ifndef FUSE_TRACKER
#define FUSE_TRACKER

#include <QTimer>
#include <QThread>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>

#include "orm_light/orm_light.h"
#include "fuse_cpp_interface.h"

  //! \brief FuseTracker
class FuseTracker : public QThread
{
  Q_OBJECT

  public:
    //! \enum My system states
  enum SystemStatus { UNKNOWN,
                      WATCHING,
                      SYNC_PULL,
                      SYNC_PUSH,
                      SYNC_PULL_REQUIRED,
                      SYNC_PUSH_REQUIRED,
                      SYNC_ALL_REQUIRED,

                      SYSTEM_STATUS_COUNT };

    //! \brief The amount of time in millaseconds to wait before timeout
  static const int TIMEOUT = 1000;

    //! \brief The amount of time for my thread to sleep
  static const int THREAD_SLEEP = 50000;

    //! \brief The max number of times to fail finding data
  static const int THREAD_FAILED = 10;
    
    //! \brief The max count for my timer count
  static const int TIMER_COUNT_MAX = 30;

    //! \brief Called to run a CLI program
  static void runCmd( QString prog );

  private:
    //! \brief My configuration to log in
  OrmLight*     Config;
    //! \brief The current system status
  SystemStatus  Status;
    //! \brief The server socket port
  QTcpServer*   Server;
    //! \brief Holds my tcp client connection
  QTcpSocket*   Client;
    //! \brief The mounted directory
  QString       Mounted;
    //! \brief This timer issues svn updates
  QTimer*       Svn_Update;
    //! \brief My port that I end up using
  int My_Port;
    //! \brief True if a timer is already started
  bool          Timer_Started;
    //! \brief Timer count, when this reaches the max, and udpate is issued
  int           Timer_Count;
    //! \brief True if my svn handle thread is running
  bool          Thread_Running;
    //! \brief Holds the data sent to me
  QStringList   Data_Read;

  public:
    //! \brief FuseTracker
  FuseTracker();

    //! \brief The status of the system
  SystemStatus status();

    //! \brief Return the configuration
  OrmLight* config();

    //! \brief Store a new configuration
  OrmLight* setConfig( OrmLight* config );

    //! \brief Open up a server port to be connected to
  bool startServer( int port );

    //! \brief Get the port that was opened
  int getPort();

    //! \brief Called when a new command is issued
  void gotCommand( FuseCppInterface::NotableAction action, QString path, QString from = QString() );

    //! \brief Store the mounted directory
  void setMounted( QString mounted );

    //! \brief Called by my thread
  void run();

  public slots:

    //! \brief Store status value
  void setStatus( SystemStatus status );

    //! \brief Connection made
  void newConnection();

    //! \brief Called when I have new data from the client
  void readyRead();

    //! \brief Called when we should udpate our svn changes
  void updateSVN();

  signals:

    //! \brief emitted when the state changes
  void statusChanged( SystemStatus status );
};

#endif
