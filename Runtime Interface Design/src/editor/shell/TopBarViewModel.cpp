#include "TopBarViewModel.h"

TopBarViewModel::TopBarViewModel(QObject* parent)
    : QObject(parent)
{}

QString TopBarViewModel::openIcon() const { return "assets/icons/Open.png"; }
QString TopBarViewModel::saveIcon() const { return "assets/icons/Save.png"; }
QString TopBarViewModel::templateIcon() const { return m_placeholder; }
QString TopBarViewModel::saveTemplateIcon() const { return "assets/icons/Save.png"; }

QString TopBarViewModel::bringToFrontIcon() const { return "assets/icons/toolbarFronted_normal.png"; }
QString TopBarViewModel::bringForwardIcon() const { return m_placeholder; }
QString TopBarViewModel::sendToBackIcon() const { return m_placeholder; }
QString TopBarViewModel::sendBackwardIcon() const { return m_placeholder; }

QString TopBarViewModel::undoIcon() const { return "assets/icons/Undo.png"; }
QString TopBarViewModel::redoIcon() const { return "assets/icons/Redo.png"; }

QString TopBarViewModel::deleteIcon() const { return "assets/icons/Remove.png"; }
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
