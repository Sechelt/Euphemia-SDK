# ################################################################ 
# CONFIG
# =======
#
# These are config options which are common among all sources.
# 
# ################################################################

# ################################################################ 
#
# Development or release?
#
# ################################################################ 

# CONFIG	+= thread warn_on release
CONFIG	+= thread warn_on debug
CONFIG 	+= c++11

unix {
	macx {
		#
		# Enable qDebug to console on OSX
		#
		contains( CONFIG, "debug" ) {
			CONFIG -= app_bundle
		}
	}
}

win32 {
	#
	# Enable qDebug to console on MS Windows
	# - we do not want this in a release as it forces use of a console
	#
	contains( CONFIG, "debug" ) {
		CONFIG += console
	}
}

# ################################################################ 
#
# Qt modules we want to use.
#
# ################################################################ 
QT += widgets
QT += printsupport
QT += xml
QT += qml
QT += svg
lessThan(QT_MAJOR_VERSION, 6) {
	QT += multimedia
}
greaterThan(QT_MAJOR_VERSION, 5) {
	QT += openglwidgets
}

#
# SQL
# - tried using Qt SQL but decided to use ODBC directly (via DATA & DATAW)
# - we use our own model (interfacing Qt & ODBC) to allow same use of Qt views
#
# QT += sql

# ################################################################ 
#
# CBD_TEXTEDITOR
#
# Configure which text editor to use for; text, sql, code, etc
#
# Choices are;
#	qt		- use a standard Qt text editor
#	scintilla	- use scintilla for Qt
#
# ################################################################ 
lessThan(QT_MAJOR_VERSION, 6) {
	CBD_TEXTEDITOR = scintilla
}
greaterThan(QT_MAJOR_VERSION, 5) {
	CBD_TEXTEDITOR = qt
}

# ################################################################ 
#
# Scintilla
#
# Edit CONFIG in \Qt4Qt5\scintilla.pro so that it is in-sync with 
# our build. ie add "staticlib debug" or "staticlib release"
# 
# ################################################################ 
contains(CBD_TEXTEDITOR,scintilla) {
	unix {
		CONFIG += qscintilla2
		LIBS += -lqscintilla2_qt5
	}
	else {
		CONFIG += qscintilla2
		INCLUDEPATH += c:\SandBox\QScintilla\Qt4Qt5
		contains( CONFIG, "debug" ) {
			LIBS += -Lc:\SandBox\QScintilla\Qt4Qt5\debug
			LIBS += -lqscintilla2_qt5d
		}
		else {
			LIBS += -Lc:\SandBox\QScintilla\Qt4Qt5\release
			LIBS += -lqscintilla2_qt5
		}
	}
}

 
 

