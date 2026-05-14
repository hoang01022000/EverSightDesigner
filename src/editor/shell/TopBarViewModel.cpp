#include "TopBarViewModel.h"

TopBarViewModel::TopBarViewModel(QObject* parent)
    : QObject(parent)
{}

QString TopBarViewModel::openIcon() const { return "assets/icons/open.svg"; }
QString TopBarViewModel::saveIcon() const { return "assets/icons/save.svg"; }
QString TopBarViewModel::templateIcon() const { return m_placeholder; }
QString TopBarViewModel::saveTemplateIcon() const { return "assets/icons/save.svg"; }

QString TopBarViewModel::bringToFrontIcon() const { return m_placeholder; }
QString TopBarViewModel::bringForwardIcon() const { return m_placeholder; }
QString TopBarViewModel::sendToBackIcon() const { return m_placeholder; }
QString TopBarViewModel::sendBackwardIcon() const { return m_placeholder; }

QString TopBarViewModel::undoIcon() const { return m_placeholder; }
QString TopBarViewModel::redoIcon() const { return m_placeholder; }

QString TopBarViewModel::deleteIcon() const { return m_placeholder; }
QString TopBarViewModel::clearIcon() const { return m_placeholder; }

QString TopBarViewModel::alignLeftIcon() const { return m_placeholder; }
QString TopBarViewModel::alignCenterIcon() const { return m_placeholder; }
QString TopBarViewModel::alignRightIcon() const { return m_placeholder; }
QString TopBarViewModel::alignTopIcon() const { return m_placeholder; }
QString TopBarViewModel::alignMiddleIcon() const { return m_placeholder; }
QString TopBarViewModel::alignBottomIcon() const { return m_placeholder; }

QString TopBarViewModel::distributeHIcon() const { return m_placeholder; }
QString TopBarViewModel::distributeVIcon() const { return m_placeholder; }

QString TopBarViewModel::toggleTopIcon() const { return m_placeholder; }
QString TopBarViewModel::toggleBottomIcon() const { return m_placeholder; }
QString TopBarViewModel::toggleLeftIcon() const { return m_placeholder; }
QString TopBarViewModel::toggleRightIcon() const { return m_placeholder; }

QString TopBarViewModel::previewIcon() const { return m_placeholder; }
QString TopBarViewModel::exportIcon() const { return m_placeholder; }
