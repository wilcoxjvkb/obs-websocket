/*
obs-websocket
Copyright (C) 2016-2017	Stéphane Lepin <stephane.lepin@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <QtCore/QUrl>

#include "../obs-websocket.h"
#include "../Config.h"
#include "../WSServer.h"

// Placed after the WSServer.h include, otherwise build fails
#include <QtQuick/QQuickView>

#include "settings-dialog.h"

#define CHANGE_ME "changeme"

SettingsDialog::SettingsDialog(QWidget* parent) :
	QDialog(parent, Qt::Dialog),
	ui(),
	// engine(new QQuickView(QUrl("qrc:/obswebsocket/forms/SettingsDialog.qml")))
	// engine(new QQuickView)
	engine(nullptr)
{
	ui.setupUi(this);

	connect(ui.authRequired, &QCheckBox::stateChanged,
		this, &SettingsDialog::AuthCheckboxChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted,
		this, &SettingsDialog::FormAccepted);

	AuthCheckboxChanged();
}

SettingsDialog::~SettingsDialog() {
	delete engine;
}

void SettingsDialog::showEvent(QShowEvent* event) {
	auto conf = GetConfig();

	ui.serverEnabled->setChecked(conf->ServerEnabled);
	ui.serverPort->setValue(conf->ServerPort);

	ui.debugEnabled->setChecked(conf->DebugEnabled);
	ui.alertsEnabled->setChecked(conf->AlertsEnabled);

	ui.authRequired->setChecked(conf->AuthRequired);
	ui.password->setText(CHANGE_ME);
}

void SettingsDialog::ToggleShowHide() {
	if (!isVisible())
		setVisible(true);
	else
		setVisible(false);
}

void SettingsDialog::AuthCheckboxChanged() {
	if (ui.authRequired->isChecked())
		ui.password->setEnabled(true);
	else
		ui.password->setEnabled(false);
}

void SettingsDialog::FormAccepted() {
	auto conf = GetConfig();

	conf->ServerEnabled = ui.serverEnabled->isChecked();
	conf->ServerPort = ui.serverPort->value();

	conf->DebugEnabled = ui.debugEnabled->isChecked();
	conf->AlertsEnabled = ui.alertsEnabled->isChecked();

	if (ui.authRequired->isChecked()) {
		if (ui.password->text() != CHANGE_ME) {
			conf->SetPassword(ui.password->text());
		}

		if (!GetConfig()->Secret.isEmpty())
			conf->AuthRequired = true;
		else
			conf->AuthRequired = false;
	}
	else
	{
		conf->AuthRequired = false;
	}

	conf->Save();

	auto server = GetServer();
	if (conf->ServerEnabled) {
		server->start(conf->ServerPort);
	} else {
		server->stop();
	}
}
