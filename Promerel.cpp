#include "Promerel.h"
#include <QEvent.h>
#include <QMessageBox>
#include <QMenu>
#include <QToolTip>
#include <QToolBar>
#include <QLayout>

#include "SetConSettingsView.h"
#include "SetElementSettingsView.h"
#include "ShowResultView.h"
#include "SetCalcSettings.h"

Promerel::Promerel(QWidget *parent)
: QMainWindow(parent),
ui_{},
scene_{},
elem_ID_{},
chosen_elem_id_{},
con_ID_{},
chosen_con_id_{},
cons_{},
elements_{},
history_{},
buffer_pos_{},
buffer_con_id_{},
buffer_elem_id_{},
is_closing_con_{},
con_stopping_history_{},
xp_{},
time_{ 1 },
is_chosing_group_{},
chosen_group_{},
add_new_con_act_{},
close_con_act_{},
delete_con_act_{},
add_new_elem_act_{},
change_elem_chars_act_{},
delete_elem_act_{},
calc_act_{},
clear_scene_{},
about_me_{},
reserve_act_{}
{
	ui_.setupUi(this);
	scene_ = new CustomScene();
	ui_.graphicsView->setScene(scene_);
	scene_->setSceneRect(0, 0, 1000, 1000);
	ui_.graphicsView->setRenderHint(QPainter::Antialiasing);
	ui_.graphicsView->setRenderHint(QPainter::HighQualityAntialiasing);
	setWindowIcon(QIcon(QTEXT(":/Promerel/pics/new/p128")));
	ui_.graphicsView->setCacheMode(QGraphicsView::CacheBackground);

	QString text{ QTEXT("Добавить новую связь") };
	add_new_con_act_ = new QAction(text, this);
	add_new_con_act_->setStatusTip(text);
	add_new_con_act_->setShortcut(Qt::Key_C);
	add_new_con_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/cn64")));

	text = QTEXT("Удалить связь");
	delete_con_act_ = new QAction(text, this);
	delete_con_act_->setStatusTip(text);
	delete_con_act_->setShortcut(Qt::Key_Delete);
	delete_con_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/cd64")));

	text = QTEXT("Закрыть связь");
	close_con_act_ = new QAction(text, this);
	close_con_act_->setStatusTip(text);
	close_con_act_->setShortcut(Qt::Key_X);
	close_con_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/cc64")));

	text = QTEXT("Добавить новый элемент");
	add_new_elem_act_ = new QAction(text, this);
	add_new_elem_act_->setStatusTip(text);
	add_new_elem_act_->setShortcut(Qt::Key_E);
	add_new_elem_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/en64")));

	text = QTEXT("Изменить свойства элемента");
	change_elem_chars_act_ = new QAction(text, this);
	change_elem_chars_act_->setStatusTip(text);
	change_elem_chars_act_->setShortcut(Qt::Key_I);
	change_elem_chars_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/ei64")));

	text = QTEXT("Удалить элемент");
	delete_elem_act_ = new QAction(text, this);
	delete_elem_act_->setStatusTip(text);
	delete_elem_act_->setShortcut(Qt::Key_Delete);
	delete_elem_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/ed64")));

	text = QTEXT("Считать...");
	calc_act_ = new QAction(text, this);
	calc_act_->setStatusTip(text);
	calc_act_->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_E));
	calc_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/calc64")));

	text = QTEXT("Удалить всё");
	clear_scene_ = new QAction(text, this);
	clear_scene_->setStatusTip(text);
	clear_scene_->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_Delete));
	clear_scene_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/d64")));

	text = QTEXT("Резервировать участок");
	reserve_act_ = new QAction(text, this);
	reserve_act_->setStatusTip(text);
	reserve_act_->setShortcut(Qt::Key_R);
	reserve_act_->setIcon(QIcon(QTEXT(":/Promerel/pics/new/res64")));

	ui_.mainToolBar->setHidden(true);
	auto *tb = new QToolBar(this);
	ui_.verticalLayout->insertWidget(0, tb);
	tb->setIconSize(QSize(32, 32));
	tb->addAction(calc_act_);
	tb->addAction(reserve_act_);
	tb->addSeparator();
	tb->addAction(add_new_elem_act_);
	tb->addAction(change_elem_chars_act_);
	tb->addAction(delete_elem_act_);
	tb->addSeparator();
	tb->addAction(add_new_con_act_);
	tb->addAction(close_con_act_);
	tb->addAction(delete_con_act_);
	tb->addSeparator();
	tb->addAction(clear_scene_);

	SetDefaultActions();

	about_me_ = new QAction(QString::fromLocal8Bit("О программе..."), this);
	auto *about = new QMenu(tr("About"), this);
	about->addAction(about_me_);
	ui_.menuBar->addMenu(about);

	connect(add_new_elem_act_, &QAction::triggered, this, &Promerel::OnAddNewElem);
	connect(change_elem_chars_act_, &QAction::triggered, this, &Promerel::OnChangeElemChars);
	connect(delete_elem_act_, &QAction::triggered, this, &Promerel::OnDeleteElem);
	connect(add_new_con_act_, &QAction::triggered, this, &Promerel::OnAddNewCon);
	connect(delete_con_act_, &QAction::triggered, this, &Promerel::OnDeleteCon);
	connect(close_con_act_, &QAction::triggered, this, &Promerel::OnCloseCon);
	connect(calc_act_, &QAction::triggered, this, &Promerel::OnCalc);
	connect(clear_scene_, &QAction::triggered, this, &Promerel::OnClearScene);
	connect(reserve_act_, &QAction::triggered, this, &Promerel::OnReserve);
	connect(about_me_, &QAction::triggered, this, &Promerel::OnAboutMe);
}

Promerel::~Promerel()
{

}

void Promerel::ShowStatusToolTip(const QString& text)
{
	ui_.statusBar->showMessage(text);
}

void Promerel::ClearStatusBar()
{
	ui_.statusBar->clearMessage();
}

QPoint Promerel::ToGlobal(QPoint pos) const
{
	return{ mapToGlobal(pos) };
}

void Promerel::ReDraw()
{
	scene_->update();
}

void Promerel::mouseDoubleClickEvent(QMouseEvent *event)
{
	//int elem_id = GetElemId();
	//int con_id = GetConId();
	//if (elem_id)
	//{
	//	UnchoseAllElems();
	//	UnchoseAllCons();
	//	buffer_elem_id_ = elem_id;
	//	chosen_elem_id_ = elem_id;
	//	GetElemById(buffer_elem_id_)->SetChosen();
	//	OnChangeElemChars();
	//	if (con_id)
	//		GetConById(con_id)->SetUnchosen();
	//}
}

void Promerel::mouseReleaseEvent(QMouseEvent *event)
{

}

void Promerel::mousePressEvent(QMouseEvent *event)
{
	if (!scene_->sceneRect().contains(event->pos()))
		return;
	if (event->button() == Qt::LeftButton)
	{
		ResetBuffer();

		ShowMenu(false, scene_->myPos);

	}
	if (event->button() == Qt::RightButton)
	{
		ResetBuffer();

		ShowMenu(true, scene_->myPos);
	}
}

void Promerel::ShowMenu(const bool is_menu, QPoint pos)
{
	QMenu menu{};
	const auto is_ctrl = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
	const int elem_id{ GetElemId() };
	buffer_elem_id_ = elem_id;
	const int con_id{ GetConId() };
	buffer_con_id_ = con_id;
	buffer_pos_ = pos;

	if (!is_chosing_group_)
	{
		UnchoseAllElems();
		UnchoseAllCons();
		//if (!chosen_group_.isEmpty())
		//	chosen_group_.clear();
	}

	if (!elem_id && !con_id)
	{
		//is_chosing_group_ = false;
		if (scene_->items().isEmpty())
		{
			if (!xp_)
			{
				xp_ = new XPoint(this, pos);
				scene_->addItem(xp_);
			}
		}
		else
		{
			if (xp_)
				xp_->SetPos(pos);
		}
		scene_->update();
		SetDefaultActions();
		if (!is_menu)
			return;
	}
	else
	{
		if (xp_)
		{
			delete xp_;
			xp_ = nullptr;
		}
		//if (is_ctrl)
		//	is_chosing_group_ = true;
	}

	int con_area_num{};
	bool node_is_busy{};
	bool node_is_chosen{ true };

	if (con_id)
	{
		auto *con = GetConById(con_id);
		if (con->IsClosing())
		{
			con_area_num = con->GetCheckedPartly();
			con->ResetPartlyFlags();
		}
		else
		{
			const auto pair = con->GetConsNodeRect();
			if (pair.second.isEmpty())
			{
				node_is_chosen = false;
			}
			else if (!con->GetBranchVect().at(pair.first - 1).isEmpty())
			{
				node_is_busy = true;
			}
		}
	}

	if (!elem_id)
	{
		if (con_id)
		{
			auto *con = GetConById(con_id);
			if (con->IsClosing())
			{
				if (con_area_num != 1 && !con->IsFixed())
				{
					if (is_menu)
						menu.addAction(add_new_elem_act_);
					add_new_elem_act_->setEnabled(true);
					add_new_con_act_->setEnabled(true);
				}
				else
				{
					add_new_elem_act_->setDisabled(true);
					add_new_con_act_->setDisabled(true);
				}
			}
			else
			{
				if (!con->IsClosed() && !node_is_busy && node_is_chosen)
				{
					if (is_menu)
						menu.addAction(add_new_elem_act_);
					add_new_elem_act_->setEnabled(true);
					add_new_con_act_->setEnabled(true);
				}
				else
				{
					add_new_elem_act_->setDisabled(true);
					add_new_con_act_->setEnabled(true);
				}
			}
		}
		else
		{
			if (scene_->items().isEmpty() || xp_)
			{
				if (is_menu)
					menu.addAction(add_new_elem_act_);
				add_new_elem_act_->setEnabled(true);
			}
			else
				add_new_elem_act_->setDisabled(true);
		}
	}
	else
	{
		ChangeChosenElem(elem_id);
		auto *el = GetElemById(elem_id);
		el->SetChosen();
		scene_->update();

		if (!GetElemById(buffer_elem_id_)->IsFixed())
		{
			if (is_menu)
			{
				menu.addAction(add_new_elem_act_);
				menu.addAction(delete_elem_act_);
			}
			add_new_elem_act_->setEnabled(true);
			delete_elem_act_->setEnabled(true);
		}
		else
		{
			add_new_elem_act_->setDisabled(true);
			delete_elem_act_->setDisabled(true);
		}
		if (is_menu)
			menu.addAction(change_elem_chars_act_);
	}
	if (is_menu)
		menu.addSeparator();
	if (!con_id)
	{
		if (scene_->items().isEmpty() || xp_)
		{
			if (is_menu)
				menu.addAction(add_new_con_act_);
			add_new_con_act_->setEnabled(true);
		}
		else
			add_new_con_act_->setDisabled(true);
		if (buffer_elem_id_)
		{
			if (!GetElemById(buffer_elem_id_)->IsFixed())
			{
				if (is_menu)
					menu.addAction(add_new_con_act_);
				add_new_con_act_->setEnabled(true);
			}
			else
				add_new_con_act_->setDisabled(true);
		}
	}
	else
	{
		auto* con = GetConById(buffer_con_id_);
		if (!elem_id)
		{
			ChangeChosenElem(0);
			ChangeChosenCon(con_id);
			con->SetChosen();
		}
		else
		{
			ChangeChosenCon(0);
			ChangeChosenElem(elem_id);
			GetElemById(elem_id)->SetChosen();
		}
		scene_->update();
		if (con_area_num != 1 && !con->IsClosed() && !node_is_busy && node_is_chosen)
		{
			if (is_menu)
				menu.addAction(add_new_con_act_);
			add_new_con_act_->setEnabled(true);
		}
		else
			add_new_con_act_->setDisabled(true);
		if (!con->IsFixed() && !con->IsClosed())
		{
			if (is_menu)
				menu.addAction(delete_con_act_);
			delete_con_act_->setEnabled(true);
		}
		else
			delete_con_act_->setDisabled(true);
		bool all_branches_empty{ true };
		for (auto branch : con->GetBranchVect())
		{
			if (!branch.isEmpty())
			{
				all_branches_empty = false;
				break;
			}
		}
		if (!con->IsClosing() && !con->IsClosed() && !all_branches_empty)
		{
			if (is_menu)
				menu.addAction(close_con_act_);
			close_con_act_->setEnabled(true);
		}
		else
			close_con_act_->setDisabled(true);
	}
	if (!scene_->items().isEmpty() && !xp_ && is_menu)
	{
		menu.addSeparator();
		menu.addAction(calc_act_);
		menu.addSeparator();
		menu.addAction(clear_scene_);
	}
	SetActionsActivity();
	if (is_menu)
		menu.exec(QCursor::pos());
}

void Promerel::ChangeChosenElem(const int id)
{
	if (chosen_elem_id_)
	{
		if (!is_chosing_group_)
			GetElemById(chosen_elem_id_)->SetUnchosen();
		else
		{
			if (chosen_group_.empty())
				chosen_group_.append({ 1, chosen_elem_id_ });
			bool obj_exist{};
			for (auto obj : chosen_group_)
			{
				if (obj.second == id)
					obj_exist = true;
			}
			if (!obj_exist)
				chosen_group_.append({ 1, id });
			return;
		}
	}
	chosen_elem_id_ = id;
}

void Promerel::ChangeChosenCon(const int id)
{
	if (chosen_con_id_)
	{
		if (!is_chosing_group_)
			GetConById(chosen_con_id_)->SetUnchosen();
		else
		{
			if (chosen_group_.empty())
			{
				if (chosen_con_id_ < 99)
					chosen_group_.append({ 2, chosen_con_id_ });
			}
			if (id < 99)
			{
				bool obj_exist{};
				for (auto obj : chosen_group_)
				{
					if (obj.second == id)
						obj_exist = true;
				}
				if (!obj_exist)
					chosen_group_.append({ 2, id });
			}
			return;
		}
	}
	chosen_con_id_ = id;
}

void Promerel::UnchoseAllElems()
{
	for (auto *el : elements_)
	{
		el->SetUnchosen();
	}
	chosen_elem_id_ = 0;
	scene_->update();
}
void Promerel::UnchoseAllCons()
{
	for (auto *con : cons_)
	{
		con->SetUnchosen();
	}
	chosen_con_id_ = 0;
}

void Promerel::SetActionsActivity()
{
	if (buffer_elem_id_ || buffer_con_id_)
		reserve_act_->setEnabled(true);
	else
		reserve_act_->setDisabled(true);
	if (buffer_elem_id_)
	{
		change_elem_chars_act_->setEnabled(true);
		close_con_act_->setDisabled(true);
	}
	else
		change_elem_chars_act_->setDisabled(true);

	if (!scene_->items().isEmpty() && !xp_)
	{
		calc_act_->setEnabled(true);
		clear_scene_->setEnabled(true);
	}
	else
	{
		calc_act_->setDisabled(true);
		clear_scene_->setDisabled(true);
	}
}

void Promerel::SetDefaultActions()
{
	if (!buffer_pos_.isNull() && xp_)
		add_new_elem_act_->setEnabled(true);
	else
		add_new_elem_act_->setDisabled(true);
	delete_elem_act_->setDisabled(true);
	if (!buffer_pos_.isNull() && xp_)
		add_new_con_act_->setEnabled(true);
	else
		add_new_con_act_->setDisabled(true);
	close_con_act_->setDisabled(true);
	delete_con_act_->setDisabled(true);
	SetActionsActivity();
	//unsetCursor();
}

void Promerel::ResetBuffer()
{
	buffer_pos_ = QPointF();
	buffer_con_id_ = 0;
	buffer_elem_id_ = 0;
	ClearStatusBar();
}

Element *Promerel::GetElemById(const int id) const
{
	for (auto el : elements_)
	{
		if (el->GetId() == id)
			return el;
	}
	return nullptr;
}

Connection *Promerel::GetConById(const int id) const
{
	for (auto con : cons_)
	{
		if (con->GetId() == id)
			return con;
	}
	return nullptr;
}

QString Promerel::GetElemVersionsCount(Element *elem) const
{
	const auto title = elem->GetTitle();
	const QString name = title.left(title.indexOf("_"));
	int count{};
	for (auto el : elements_)
	{
		if (el->GetTitle().contains(!name.isEmpty() ? name : title))
			++count;
	}
	return QString("%1_%2").arg(count > 1 ? name : title).arg(count);
}

void Promerel::OnReserve()
{
	//UnchoseAllElems();
	//UnchoseAllCons();
	//for (auto obj : chosen_group_)
	//{
	//	if (obj.first == 1)
	//	{
	//		auto *cur_el = GetElemById(obj.second);
	//		const auto new_title = GetElemVersionsCount(cur_el);
	//		auto *new_el = new Element(cur_el, ++elem_ID_, new_title);
	//		elements_.append(new_el);
	//		scene_->addItem(new_el);
	//		new_el->setPos(cur_el->pos());
	//		new_el->MoveIt(-30);
	//	}
	//}
	//ReDraw();
}

void Promerel::OnAboutMe()
{
	auto text = QTEXT("Program for Measuring Reliability (Promerel)\n");
	text += QTEXT("Разработчик: ст.гр.ИС-13, кафедры ЭИИС, Института ИТАСУ\nЕвдаков Максим Андреевич\n");
	text += QTEXT("С вопросами и предложениями можно обращаться по:\n");
	text += QTEXT("e-mail: max_evdakov@mail.ru\n");
	text += QTEXT("vk.com: evdakovm\n\n");
	text += QTEXT("© М.А.Евдаков, 2017");
	QMessageBox::information(this, QTEXT("О программе"), text);
}

void Promerel::OnAddNewElem()
{
	auto *set = new SetElementSettingsView();
	if (!set->exec())
		return;
	bool is_new_tree{ scene_->items().empty() };
	QPointF poss{};
	int bel_con_id{ -1 };
	int br_num{ -1 };
	if (buffer_elem_id_ > 0)
	{
		auto *el = GetElemById(buffer_elem_id_);
		poss = el->GetElemsExitPoint();
		const PairInt pair = el->GetBelConIDNum();
		if (pair.second != -1)
		{
			GetConById(pair.first)->SetClosePoint(pair.second, poss.toPoint() + QPointF(80, 0));
			bel_con_id = pair.first;
			br_num = pair.second;
		}
		el->SetFixed();
		if (!con_stopping_history_)
		{
			MakeHistory(1, elem_ID_ + 1);
		}
		else
		{
			GetConById(pair.first)->MakeBranchHistory(pair.second, 1, elem_ID_ + 1);
		}
	}
	else if (buffer_con_id_ > 0)
	{
		auto *con = GetConById(buffer_con_id_);
		QPair<int, QRect> pair{};
		if (!con->IsClosing())
		{
			pair = con->GetConsNodeRect();
		}
		else
		{
			pair = con->GetFirstNode();
		}
		poss = pair.second.topRight() + QPointF(0, 8);
		if (!con->IsClosing())
		{
			bel_con_id = buffer_con_id_;
			br_num = pair.first;
			con->SetClosePoint(br_num, poss.toPoint() + QPointF(80, 0));
			con->MakeBranchHistory(br_num, 1, elem_ID_ + 1);
		}
		else
		{
			if (!con_stopping_history_)
			{
				MakeHistory(1, elem_ID_ + 1);
			}
			else
			{
				const PairInt p = GetConById(buffer_con_id_ / 100)->GetBelConIDNum();
				if (p.second > 0)
				{
					bel_con_id = p.first;
					br_num = p.second;
					auto *con_father = GetConById(p.first);
					con_father->MakeBranchHistory(p.second, 1, elem_ID_ + 1);
					con_father->SetClosePoint(p.second, poss.toPoint() + QPointF(80, 0));
				}
			}
		}
		con->SetFixed();
	}
	else
	{
		is_new_tree = true;
		poss = buffer_pos_;
		MakeHistory(1, elem_ID_ + 1);
	}
	const auto title = set->GetElemTitle();
	const auto data = set->GetData();
	PairInt prev_body{ buffer_elem_id_ ? 1 : 2, buffer_elem_id_ ? buffer_elem_id_ : buffer_con_id_ };
	auto *el = new Element(this, ++elem_ID_, is_new_tree, PairInt(bel_con_id, br_num), prev_body, poss, title, data);
	scene_->addItem(el);
	el->setPos(poss);
	elements_.append(el);
	UnchoseAllElems();
	UnchoseAllCons();
	ResetBuffer();
	if (xp_)
	{
		delete xp_;
		xp_ = nullptr;
	}
	SetDefaultActions();
	scene_->update();
}

void Promerel::OnChangeElemChars()
{
	GetElemById(buffer_elem_id_)->ChangeChars();
}

void Promerel::OnAddNewCon()
{
	int branch_count{};
	if (!is_closing_con_)
	{
		auto *set = new SetConSettingsView(this);
		if (!set->exec())
			return;
		branch_count = set->GiveBranchCount();
	}
	bool is_new_tree{ scene_->items().empty() };
	QVector<QPointF> poss{};
	int bel_con_id{ -1 };
	int br_num{ -1 };
	if (buffer_elem_id_ > 0)
	{
		auto *el = GetElemById(buffer_elem_id_);
		const PairInt pair = el->GetBelConIDNum();
		if (pair.second != -1)
		{
			bel_con_id = pair.first;
			br_num = pair.second;
		}
		poss.append(el->GetElemsExitPoint().toPoint());
		el->SetFixed();
		if (!con_stopping_history_)
		{
			con_stopping_history_ = con_ID_ + 1;
			MakeHistory(2, con_ID_ + 1);
		}
		else
		{
			GetConById(pair.first)->MakeBranchHistory(pair.second, 2, con_ID_ + 1);
		}
	}
	else if (buffer_con_id_ > 0)
	{
		if (!is_closing_con_)
		{
			auto *con = GetConById(buffer_con_id_);
			const auto pair = con->GetConsNodeRect();
			poss.append(pair.second.topRight() + QPointF(0, 8));
			con->SetFixed();
			if (!con->IsClosing())
			{
				con->MakeBranchHistory(pair.first, 2, con_ID_ + 1);
				bel_con_id = buffer_con_id_;
				br_num = pair.first;
			}
			else
			{
				if (!con_stopping_history_)
				{
					MakeHistory(2, con_ID_ + 1);
					con_stopping_history_ = con_ID_ + 1;
				}
				else
				{
					const auto p = GetConById(buffer_con_id_ / 100)->GetBelConIDNum();
					GetConById(p.first)->MakeBranchHistory(p.second, 2, con_ID_ + 1);
				}
				auto *con_father = GetConById(buffer_con_id_ / 100);
				const auto p = con_father->GetBelConIDNum();
				if (p.second > 0)
				{
					bel_con_id = p.first;
					br_num = p.second;
				}
			}
		}
		else
		{
			auto *con = GetConById(buffer_con_id_);
			const auto point_vect = con->GetVectCloseRectGlobal();
			QPointF the_farest_point{};
			int max_x = std::numeric_limits<int>::min();
			for (auto point : point_vect)
			{
				if (point.x() > max_x)
				{
					max_x = point.x();
					the_farest_point = point;
				}
				poss.append(point);
			}
			if (con_stopping_history_ == buffer_con_id_)
			{
				con_stopping_history_ = 0;
			}
			const auto pair = con->GetBelConIDNum();
			if (pair.second > 0)
			{
				auto *con_father = GetConById(pair.first);
				const QPointF point{ QPointF(the_farest_point.x() + 32, con->GetStartPos().y()) };
				con_father->SetClosePoint(pair.second, point);
			}
		}
	}
	else
	{
		is_new_tree = true;
		poss.append(buffer_pos_);
		MakeHistory(2, con_ID_ + 1);
		con_stopping_history_ = con_ID_ + 1;
	}
	if (is_closing_con_)
	{
		branch_count = GetConById(buffer_con_id_)->GetBranchCount();
	}
	PairInt prev_body{ buffer_elem_id_ ? 1 : 2, buffer_elem_id_ ? buffer_elem_id_ : buffer_con_id_ };
	if (is_closing_con_ || (buffer_con_id_ && (buffer_con_id_ > 0 ? !GetConById(buffer_con_id_)->IsClosing() : 1)))
	{
		prev_body.first = 0;
		prev_body.second = 0;
	}
	auto *con = new Connection(this, is_closing_con_ ? buffer_con_id_ * 100 : ++con_ID_, is_new_tree, prev_body, PairInt(bel_con_id, br_num), is_closing_con_, poss, branch_count);
	scene_->addItem(con);
	con->setPos(poss.front());
	if (is_closing_con_)
		con->MakeBeauty();
	cons_.append(con);
	if (is_closing_con_)
	{
		SetConClosed(buffer_con_id_);
		is_closing_con_ = false;
	}
	UnchoseAllElems();
	UnchoseAllCons();
	ResetBuffer();
	if (xp_)
	{
		delete xp_;
		xp_ = nullptr;
	}
	SetDefaultActions();
	scene_->update();
}

void Promerel::MakeHistory(const int obj_type, const int id)
{
	history_.append(PairInt(obj_type, id));
}

void Promerel::SetConClosed(const int id)
{
	GetConById(id)->SetClosed();
	for (auto el : elements_)
	{
		const int con_id = el->GetBelConIDNum().first;
		if (con_id == id && !el->IsFixed())
			el->SetFixed();
	}
}

bool Promerel::IsCalculable() const
{
	if (con_stopping_history_)
		return false;
	else
		return true;
}

void Promerel::OnCalc()
{
	if (!IsCalculable())
	{
		QMessageBox::warning(this, QTEXT("Ошибка"), QTEXT("Расчёт невозможен. Не все связи закрыты!"));
		return;
	}

	SetCalcSettings set(this);
	if (!set.exec())
		return;


	double sys_rel{ 1 };
	double sys_in{};
	const auto time = set.GetTime();
	for (auto obj : history_)
	{
		if (obj.first == 1)
		{
			auto *el = GetElemById(obj.second);
			const auto rel = el->GetReliability();
			const auto in = el->GetIntensity();
			if (rel)
				sys_rel *= rel;
			if (in)
				sys_in += in;
		}
		else
		{
			auto pair = CalcCon(obj.second, time);
			sys_rel *= pair.first;
			sys_in += pair.second;
		}
	}

	auto *sh = new ShowResultView(this, sys_in, set.GetSets(), time ? sys_rel : set.GetRel(), time);
	sh->exec();
}

QPair<double, double> Promerel::CalcCon(const int id, const double time)
{
	double reliability{ 1. };
	double intensity{};
	auto *con = GetConById(id);
	int i{};
	for (auto branch : con->GetBranchVect())
	{
		double branch_in{};
		float branch_rel{ 1 };
		++i;
		for (auto obj : branch)
		{
			if (obj.first == 2)
			{
				auto pair = CalcCon(obj.second, time);
				branch_rel *= pair.first;
				branch_in += pair.second;
			}
			else
			{
				auto *el = GetElemById(obj.second);
				if (time)
				{
					branch_rel *= exp(-el->GetIntensity()*time);
					branch_in += el->GetIntensity();
				}
				else
				{
					const auto r = el->GetReliability();
					const auto in = el->GetIntensity();
					if (r)
						branch_rel *= r;
					else if (in)
						branch_in += in;
				}
			}
		}
		if (branch_rel != 1)
			reliability *= (1 - branch_rel);
		if (branch_in != 0)
			intensity += 1 / (i * branch_in);
	}
	return{ 1 - reliability, intensity != 0 ? 1 / intensity : 0 };
}

void Promerel::OnDeleteElem()
{
	for (auto el : elements_)
	{
		if (el->GetId() == buffer_elem_id_)
		{
			const auto p = el->GetBelConIDNum();
			if (p.second > 0 && p.first < 100)
			{
				auto *con = GetConById(p.first);
				con->DeleteLastClosePoint(p.second);
				const auto branches = con->GetBranchVect();
				if (branches.at(p.second - 1).size() == 1)
				{
					bool success{ true };
					for (int i = 0; i < con->GetBranchCount() - 1; ++i)
					{
						if (i != p.second - 1)
						{
							if (!branches.at(i).isEmpty())
							{
								success = false;
							}
						}
					}
					if (success)
					{
						con->SetUnfixed();
					}
				}
				con->RemoveObjOnBranch(p.second, 1, buffer_elem_id_);
			}
			if (el->HasPrevObj())
			{
				const PairInt pair = el->GetPrevObject();
				if (pair.first == 1)
				{
					GetElemById(pair.second)->SetUnfixed();
				}
				else if (pair.first == 2)
				{
					GetConById(pair.second)->SetUnfixed();
				}
			}
			ChangeChosenElem(0);
			scene_->update();
			delete el;
			elements_.removeOne(el);
			for (auto obj : history_)
			{
				if (obj.first == 1 && obj.second == buffer_elem_id_)
				{
					history_.removeOne(obj);
				}
			}
			ResetBuffer();
			SetDefaultActions();
			return;
		}
	}
}

void Promerel::OnDeleteCon()
{
	for (auto con : cons_)
	{
		if (con->GetId() == buffer_con_id_)
		{
			if (con->IsClosing())
			{
				if (!con_stopping_history_)
				{
					con_stopping_history_ = buffer_con_id_ / 100;
				}
				auto vect = GetConById(buffer_con_id_ / 100)->GetBranchVect();
				for (auto branch : vect)
				{
					if (!branch.isEmpty())
					{
						const int obj_type = branch.back().first;
						const int id = branch.back().second;
						if (obj_type == 1)
						{
							GetElemById(id)->SetUnfixed();
						}
						else if (obj_type == 2)
						{
							GetConById(id)->SetUnfixed();
						}
					}
				}
				GetConById(buffer_con_id_ / 100)->SetOpen();
			}
			else
			{
				if (con_stopping_history_ == buffer_con_id_)
				{
					con_stopping_history_ = 0;
				}
				const auto p = con->GetBelConIDNum();
				if (p.second > 0)
				{
					auto *con_father = GetConById(p.first);
					con->DeleteLastClosePoint(p.second);
					const auto branches = con_father->GetBranchVect();
					if (branches.at(p.second - 1).size() == 1)
					{
						bool success{ true };
						for (int i = 0; i < con_father->GetBranchCount() - 1; ++i)
						{
							if (i != p.second - 1)
							{
								if (!branches.at(i).isEmpty())
								{
									success = false;
								}
							}
						}
						if (success)
						{
							con_father->SetUnfixed();
						}
					}
					con_father->RemoveObjOnBranch(p.second, 2, buffer_con_id_);
				}
				if (con->HasPrevObj())
				{
					const PairInt pair = con->GetPrevObject();
					if (pair.first == 1)
					{
						GetElemById(pair.second)->SetUnfixed();
					}
					else if (pair.first == 2)
					{
						GetConById(pair.second)->SetUnfixed();
					}
				}
			}
			ChangeChosenElem(0);
			scene_->update();
			delete con;
			cons_.removeOne(con);
			for (auto obj : history_)
			{
				if (obj.first == 2 && obj.second == buffer_con_id_)
				{
					history_.removeOne(obj);
				}
			}
			ResetBuffer();
			SetDefaultActions();
			return;
		}
	}
}

void Promerel::OnCloseCon()
{
	if (!CanICloseThisCon(buffer_con_id_))
	{
		QMessageBox::warning(this, QTEXT("Ошибка"), QTEXT("Вы не можете закрыть эту связь.\nОстались незакрытые связи внутри неё!"));
		return;
	}
	is_closing_con_ = true;
	OnAddNewCon();
}

bool Promerel::CanICloseThisCon(const int con_id) const
{
	auto *con = GetConById(con_id);
	for (auto branch : con->GetBranchVect())
	{
		for (auto obj : branch)
		{
			if (obj.first == 2)
			{
				bool res = GetConById(obj.second)->HasConClosedCons();
				if (!res)
					return false;
			}
		}
	}
	return true;
}

void Promerel::OnClearScene()
{
	const int ans = QMessageBox::question(this, QTEXT("Внимание"), QTEXT("Вы уверены, что хотите удалить все объекты?"), QTEXT("Да"), QTEXT("Нет"));
	if (ans)
		return;
	ChangeChosenElem(0);
	for (auto el : elements_)
	{
		delete el;
	}
	elements_.clear();
	ChangeChosenCon(0);
	for (auto con : cons_)
	{
		delete con;
	}
	cons_.clear();
	history_.clear();
	con_stopping_history_ = 0;
	con_ID_ = 0;
	elem_ID_ = 0;
	ResetBuffer();
	SetDefaultActions();
}

int Promerel::GetConId()
{
	int id{};
	if (!cons_.empty())
	{
		for (auto con : cons_)
		{
			if (con->IsChecked())
			{
				id = con->GetId();
				if (id > 0)
					break;
			}
		}
	}
	ResetConsChecked();
	return id;
}

int Promerel::GetElemId()
{
	int id{};
	if (!elements_.empty())
	{
		for (auto el : elements_)
		{
			if (el->IsChecked())
			{
				id = el->GetId();
				if (id > 0)
					break;
			}
		}
	}
	ResetElemsChecked();
	return id;
}

void Promerel::ResetElemsChecked()
{
	for (auto *el : elements_)
	{
		el->ResetFlag();
	}
}

void Promerel::ResetConsChecked()
{
	for (auto *con : cons_)
	{
		con->ResetFlag();
	}
}