#include "opt_status.h"
#include "common.h"
#include "iconwidget.h"
#include "psioptions.h"

#include <limits.h>
#include <QButtonGroup>
#include <QMessageBox>
#include <QWhatsThis>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QInputDialog>

#include "ui_opt_status.h"

class OptStatusUI : public QWidget, public Ui::OptStatus
{
public:
	OptStatusUI() : QWidget() { setupUi(this); }
};

//----------------------------------------------------------------------------
// OptionsTabStatus
//----------------------------------------------------------------------------

OptionsTabStatus::OptionsTabStatus(QObject *parent)
: OptionsTab(parent, "status", "", tr("Status"), tr("Status preferences"), "psi/status")
{
	w = 0;
}

OptionsTabStatus::~OptionsTabStatus()
{
}

QWidget *OptionsTabStatus::widget()
{
	if ( w )
		return 0;

	w = new OptStatusUI();
	OptStatusUI *d = (OptStatusUI *)w;

	QString s = tr("Makes Psi automatically set your status to \"away\" if your"
		" computer is idle for the specified amount of time.");
	d->ck_asAway->setWhatsThis(s);
	d->sb_asAway->setWhatsThis(s);
	s = tr("Makes Psi automatically set your status to \"extended away\" if your"
		" computer is idle for the specified amount of time.");
	d->ck_asXa->setWhatsThis(s);
	d->sb_asXa->setWhatsThis(s);
	s = tr("Makes Psi automatically set your status to \"offline\" if your"
		" computer is idle for the specified amount of time."
		"  This will disconnect you from the XMPP server.");
	if (!PsiOptions::instance()->getOption("options.ui.menu.status.xa").toBool()) {
		d->ck_asXa->hide();
		d->sb_asXa->hide();
		d->lb_asXa->hide();
	}
	d->ck_asOffline->setWhatsThis( s);
	d->sb_asOffline->setWhatsThis( s);

	return w;

}

void OptionsTabStatus::applyOptions()
{
	if ( !w )
		return;

	OptStatusUI *d = (OptStatusUI *)w;

	PsiOptions::instance()->setOption("options.status.auto-away.away-after", d->sb_asAway->value());
	PsiOptions::instance()->setOption("options.status.auto-away.not-availible-after", d->sb_asXa->value());
	PsiOptions::instance()->setOption("options.status.auto-away.offline-after", d->sb_asOffline->value());
	PsiOptions::instance()->setOption("options.status.auto-away.use-away", d->ck_asAway->isChecked());
	PsiOptions::instance()->setOption("options.status.auto-away.use-not-availible", d->ck_asXa->isChecked());
	PsiOptions::instance()->setOption("options.status.auto-away.use-offline", d->ck_asOffline->isChecked());
	
	foreach (QString name, deletedPresets) {
		QString base = PsiOptions::instance()->mapLookup("options.status.presets", name);
		PsiOptions::instance()->removeOption(base , true);
	}
	deletedPresets.clear();
	foreach (QString name, dirtyPresets.toList() + newPresets.keys()) {
		StatusPreset sp;
		if (newPresets.contains(name)) {
			sp = newPresets[name];
		} else {
			sp = presets[name];
		}
		PsiOptions *o = PsiOptions::instance();
		sp.toOptions(o);
	}
	dirtyPresets.clear();
	presets.unite(newPresets);
	newPresets.clear();
}

void OptionsTabStatus::restoreOptions()
{
	if ( !w )
		return;

	OptStatusUI *d = (OptStatusUI *)w;

	d->sb_asAway->setMinimum(0);
	d->sb_asAway->setMaximum(INT_MAX);
	d->sb_asAway->setValue( PsiOptions::instance()->getOption("options.status.auto-away.away-after").toInt() );
	d->sb_asXa->setMinimum(0);
	d->sb_asXa->setMaximum(INT_MAX);
	d->sb_asXa->setValue( PsiOptions::instance()->getOption("options.status.auto-away.not-availible-after").toInt() );
	d->sb_asOffline->setMinimum(0);
	d->sb_asOffline->setMaximum(INT_MAX);
	d->sb_asOffline->setValue( PsiOptions::instance()->getOption("options.status.auto-away.offline-after").toInt() );
	/*if (PsiOptions::instance()->getOption("options.status.auto-away.away-after").toInt() <= 0 )
		PsiOptions::instance()->getOption("options.status.auto-away.use-away").toBool() = false;
	if (PsiOptions::instance()->getOption("options.status.auto-away.not-availible-after").toInt() <= 0 )
		PsiOptions::instance()->getOption("options.status.auto-away.use-not-availible").toBool() = false;
	if(d->opt.asOffline <= 0)
		PsiOptions::instance()->getOption("options.status.auto-away.use-offline").toBool() = false;*/
	d->ck_asAway->setChecked( PsiOptions::instance()->getOption("options.status.auto-away.use-away").toBool() );
	d->ck_asXa->setChecked( PsiOptions::instance()->getOption("options.status.auto-away.use-not-availible").toBool() );
	d->ck_asOffline->setChecked( PsiOptions::instance()->getOption("options.status.auto-away.use-offline").toBool() );

	QStringList presetNames;
	
	foreach(QVariant name, PsiOptions::instance()->mapKeyList("options.status.presets")) {
		QString base =  PsiOptions::instance()->mapLookup("options.status.presets", name.toString());
		StatusPreset sp;
		sp.setName(name.toString());
		sp.setMessage(PsiOptions::instance()->getOption(base+".message").toString());
		if (PsiOptions::instance()->getOption(base+".force-priority").toBool()) {
			sp.setPriority(PsiOptions::instance()->getOption(base+".priority").toInt());
		}

		XMPP::Status status;
		status.setType(PsiOptions::instance()->getOption(base+".status").toString());
		sp.setStatus(status.type());
		
		presets[name.toString()] = sp;
		presetNames += name.toString();
    }
}

void OptionsTabStatus::setData(PsiCon *, QWidget *parentDialog)
{
	parentWidget = parentDialog;
}
