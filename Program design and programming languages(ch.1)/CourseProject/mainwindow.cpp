#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "in_out_file.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btn_Edit->setVisible(false);
    ui->btn_Save->setVisible(false);
    ui->btn_Save_2->setVisible(false);
    ui->btn_Remove->setVisible(false);
    ui->gv_Area->setVisible(false);
    ui->lab_HELLO->setVisible(true);
    ui->lineEdit->setPlaceholderText(" Поиск");
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(Timer()));
    _timer->start(1000);//Запускаем таймер(часы)
    ui->action->setShortcut(tr("Ctrl+z"));
    ui->action_2->setShortcut(tr("Ctrl+x"));

    cout_file(_family, "Family.txt");//Читаем из файйла в лист
    cout_file(_friend, "Friend.txt");//Читаем из файла в лист
    cout_file(_colleague, "Colleague.txt");//Читаем из файла в лист
    cout_file(_unknows, "Unknown.txt");//Читаем из файла в лист

    this->Filling_Tree();//Заполняем дерево
    this->Filling_ListWidget();//Заполняем список объектов
}

void MainWindow::on_btn_ADD_clicked()
{
    ui->btn_Save->setVisible(true);
    ui->btn_ADD->setEnabled(false);
    ui->btn_Save_2->setVisible(false);
    ui->btn_Edit->setVisible(false);
    ui->btn_Remove->setVisible(false);
    ui->lab_HELLO->setVisible(false);
    ui->gv_Area->setVisible(true);
    this->deleteElementsDynamic();
    this->deleteElementsStatic();

    this->_Path_of_File = "default.png";//Картинка по умолчанию

    this->display_interface_for_data_entry();//Отображаем поля для ввода данных

    Rb_Unknown->setChecked(true);

    CBox_CheckHome->addItem(" Частный дом");
    CBox_CheckHome->addItem(" Квартира");

    line_AdressFlat->setVisible(false);

    connect(CBox_CheckHome, SIGNAL(currentIndexChanged(int)), this, SLOT(changed_Enabled_AdressFlat()));
    connect(btn_Image, SIGNAL(clicked()), this, SLOT(on_btn_Image_clicked()));
    connect(Rb_Family, SIGNAL(toggled(bool)), this, SLOT(slot_Family()));
    connect(Rb_friend, SIGNAL(toggled(bool)), this, SLOT(slot_Friend()));
    connect(Rb_colleague, SIGNAL(toggled(bool)), this, SLOT(slot_Colleague()));
    connect(Rb_Unknown, SIGNAL(toggled(bool)), this, SLOT(slot_Unknown()));
}

void MainWindow::on_btn_Image_clicked()
{
    this->_Path_of_File = QFileDialog::getOpenFileName(this, tr("Open File"), "E://", "png files (*.*);;jpg files (*.jpg)");//Выбираем картинку
    if(this->_Path_of_File == "")
    {
        btn_Image->setIcon(QIcon("default.png"));
        this->_Path_of_File = "default.png";
    }
    else btn_Image->setIcon(QIcon(this->_Path_of_File));
}

QString MainWindow::save_image(QString path, QString n, QString s, QString p)//Сохраняем картинку
{
    QPixmap px;
    px.load(path);
    QString temp = "фото/" + n[0] + n[n.size()-1] + p.mid(5,2)+ s[0] + s[s.size()-1] + p.mid(13,2) + ".png";
    px.save(temp);
    return temp;
}

void MainWindow::changed_Enabled_AdressFlat()
{
    if(CBox_CheckHome->currentIndex() == 1)
        line_AdressFlat->setVisible(true);
    else
        line_AdressFlat->setVisible(false);
}

void MainWindow::data_entry_verification()//Проверяем правильность заполнения полей
{
    _check_names(line_Name->text(), 25);
    _check_names(line_SecondName->text(), 25);
    _check_names(line_FatherName->text(), 25);
    _chech_number(line_Number->text());
    _check_names(line_AdressCity->text(), 25);
    _check_adress(line_AdressStreet->text(), 25);
    _check_adress(line_AdressHome->text(), 8);
    if(CBox_CheckHome->currentIndex() == 1)
        _check_adress(line_AdressFlat->text(), 8);
    if(line_Date)
        _check_date(line_Date->text());
    if(line_Post)
        _check_names(line_Post->text(), 25);
    if(line_FamilyPosition)
        _check_names(line_FamilyPosition->text(), 25);
}

void MainWindow::on_btn_Save_clicked()
{
    try
    {
        this->data_entry_verification();//Проверяем правильность заполнения полей
        if(Rb_Unknown->isChecked())//дальше сохраняем объект
        {
            AboutHuman _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();          
            _tempHum.SetWho_is_it("Unknown");
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _unknows.push_back(_tempHum);
            _all.push(&_unknows[_unknows.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Unknown.txt");

            this->Filling_ListWidget();
            _actions.push_back(1);
            _type.push_back(4);
        }
        else if(Rb_Family->isChecked())
        {
            Family _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();
            _tempHum.SetFamilyPosition(line_FamilyPosition->text());
            _tempHum.SetWho_is_it("Family");
            _tempHum.Set_Data(line_Date->text());
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _family.push_back(_tempHum);
            _all.push(&_family[_family.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Family.txt");

            this->Filling_ListWidget();
            _actions.push_back(1);
            _type.push_back(1);
        }
        else if(Rb_friend->isChecked())
        {
            Friend _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();
            _tempHum.SetTypeOfFriend(CBox_Friends->currentIndex() + 1);
            _tempHum.SetWho_is_it("Friend");
            _tempHum.Set_Data(line_Date->text());
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _friend.push_back(_tempHum);
            _all.push(&_friend[_friend.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Friend.txt");

            this->Filling_ListWidget();
            _actions.push_back(1);
            _type.push_back(2);
        }
        else if(Rb_colleague->isChecked())
        {
            Colleague _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();
            _tempHum.SetPost(line_Post->text());
            _tempHum.SetWho_is_it("Colleague");
            _tempHum.Set_Data(line_Date->text());
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _colleague.push_back(_tempHum);
            _all.push(&_colleague[_colleague.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Colleague.txt");

            this->Filling_ListWidget();
            _actions.push_back(1);
            _type.push_back(3);
        }
        ui->gv_Area->setVisible(false);
        this->deleteElementsDynamic();
        this->deleteElementsStatic();
        ui->btn_Save->setVisible(false);
        ui->btn_ADD->setEnabled(true);
        ui->lab_HELLO->setVisible(true);
    }
    catch(ExceptionInput& ex)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(ex.what() + "\nКод ошибки: " + ex.Code());
        msgBox.exec();//Выдаем ошибку если она есть
        return;
    }
}

void MainWindow::on_btn_Remove_clicked()//Удаляем объект
{
    if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Family")
    {
        _data_Deleted_Family.push_back(*dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]]));
        _family.removeOne(*dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]]));
        this->Filling_Tree();
        write_to_new_file(_family.begin(), _family.end(), "Family.txt");
        _actions.push_back(2);
        _type.push_back(1);
    }
    else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Friend")
    {
        _data_Deleted_Friend.push_back(*dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]]));
        _friend.removeOne(*dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]]));
        this->Filling_Tree();
        write_to_new_file(_friend.begin(), _friend.end(), "Friend.txt");
        _actions.push_back(2);
        _type.push_back(2);
    }
    else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Colleague")
    {
        _data_Deleted_Colleague.push_back(*dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]]));
        _colleague.removeOne(*dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]]));
        this->Filling_Tree();
        write_to_new_file(_colleague.begin(), _colleague.end(), "Colleague.txt");
        _actions.push_back(2);
        _type.push_back(3);
    }
    else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Unknown")
    {
        _data_Deleted_AboutHuman.push_back(*(dynamic_cast<AboutHuman*>(_all[_index[ui->list_widget->currentRow()]])));
        _unknows.removeOne(*(dynamic_cast<AboutHuman*>(_all[_index[ui->list_widget->currentRow()]])));
        this->Filling_Tree();
        write_to_new_file(_unknows.begin(), _unknows.end(), "Unknown.txt");
        _actions.push_back(2);
        _type.push_back(4);
    }
    ui->gv_Area->setVisible(false);
    this->deleteElementsDynamic();
    this->deleteElementsStatic();
    ui->btn_Remove->setVisible(false);
    ui->btn_Edit->setVisible(false);
    ui->lab_HELLO->setVisible(true);
    this->Filling_ListWidget();
}

void MainWindow::on_btn_Edit_clicked()//Редактируем объект
{
    ui->btn_Edit->setVisible(false);
    ui->btn_Save->setVisible(false);
    ui->btn_Save_2->setVisible(true);
    ui->btn_Remove->setVisible(false);
    ui->lab_HELLO->setVisible(false);
    ui->gv_Area->setVisible(true);
    this->deleteElementsDynamic();
    this->deleteElementsStatic();
    this->_Path_of_File = _all[_index[ui->list_widget->currentRow()]]->GetPath_to_photo();


    this->display_interface_for_data_entry();

    CBox_CheckHome->addItem(" Частный дом");
    CBox_CheckHome->addItem(" Квартира");

    line_AdressFlat->setVisible(false);

    line_Name->setText(_all[_index[ui->list_widget->currentRow()]]->GetName());
    line_SecondName->setText(_all[_index[ui->list_widget->currentRow()]]->GetSecondName());
    line_FatherName->setText(_all[_index[ui->list_widget->currentRow()]]->GetFatherName());
    line_Number->setText(_all[_index[ui->list_widget->currentRow()]]->GetNumberOfPhone());
    line_AdressCity->setText(_all[_index[ui->list_widget->currentRow()]]->GetAdressCity());
    line_AdressStreet->setText(_all[_index[ui->list_widget->currentRow()]]->GetAdressStreet());
    line_AdressHome->setText(_all[_index[ui->list_widget->currentRow()]]->GetAdressHome());
    if(_all[_index[ui->list_widget->currentRow()]]->GetAdressFlat() != "")
    {
        line_AdressFlat->setText(_all[_index[ui->list_widget->currentRow()]]->GetAdressFlat());
        CBox_CheckHome->setCurrentIndex(1);
        line_AdressFlat->setVisible(true);
    }
    if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Unknown")
    {
        Rb_Unknown->setChecked(true);
        this->slot_Unknown();
    }
    else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Family")
    {
        Rb_Family->setChecked(true);
        this->slot_Family();
        line_Date->setText(dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]])->Get_Data_s());
        line_FamilyPosition->setText(dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]])->GetFamilyPosition());
    }
    else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Friend")
    {
        Rb_friend->setChecked(true);
        this->slot_Friend();
        line_Date->setText(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->Get_Data_s());
        if(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->GetTyoeOfFriend() == "School Friend")
            CBox_Friends->setCurrentIndex(0);
        else if(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->GetTyoeOfFriend() == "Childhood Friend")
            CBox_Friends->setCurrentIndex(1);
        else if(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->GetTyoeOfFriend() == "University Friend")
            CBox_Friends->setCurrentIndex(2);
        else if(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->GetTyoeOfFriend() == "Work Friend")
            CBox_Friends->setCurrentIndex(3);

    }
    else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Colleague")
    {
        Rb_colleague->setChecked(true);
        this->slot_Colleague();
        line_Date->setText(dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]])->Get_Data_s());
        line_Post->setText(dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]])->GetPost());
    }

    connect(CBox_CheckHome, SIGNAL(currentIndexChanged(int)), this, SLOT(changed_Enabled_AdressFlat()));
    connect(btn_Image, SIGNAL(clicked()), this, SLOT(on_btn_Image_clicked()));
    connect(Rb_Family, SIGNAL(toggled(bool)), this, SLOT(slot_Family()));
    connect(Rb_friend, SIGNAL(toggled(bool)), this, SLOT(slot_Friend()));
    connect(Rb_colleague, SIGNAL(toggled(bool)), this, SLOT(slot_Colleague()));
    connect(Rb_Unknown, SIGNAL(toggled(bool)), this, SLOT(slot_Unknown()));
}

void MainWindow::on_btn_Save_2_clicked()//Сохраняем редактированный объект
{
    try
    {
        this->data_entry_verification();//Проверяем правильность заполнения полей
        if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Family")
        {
            _actions.push_back(3);
            _type.push_back(1);
            _data_Deleted_Family.push_back(*(dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]])));
            _family.removeOne(*dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]]));
            this->Filling_Tree();
            write_to_new_file(_family.begin(), _family.end(), "Family.txt");
        }
        else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Friend")
        {
            _actions.push_back(3);
            _type.push_back(2);
            _data_Deleted_Friend.push_back(*(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])));
            _friend.removeOne(*dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]]));
            this->Filling_Tree();
            write_to_new_file(_friend.begin(), _friend.end(), "Friend.txt");
        }
        else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Colleague")
        {
            _actions.push_back(3);
            _type.push_back(3);
            _data_Deleted_Colleague.push_back(*(dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]])));
            _colleague.removeOne(*dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]]));
            this->Filling_Tree();
            write_to_new_file(_colleague.begin(), _colleague.end(), "Colleague.txt");
        }
        else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Unknown")
        {
            _actions.push_back(3);
            _type.push_back(4);
            _data_Deleted_AboutHuman.push_back(*(dynamic_cast<AboutHuman*>(_all[_index[ui->list_widget->currentRow()]])));
            _unknows.removeOne(*(dynamic_cast<AboutHuman*>(_all[_index[ui->list_widget->currentRow()]])));
            this->Filling_Tree();
            write_to_new_file(_unknows.begin(), _unknows.end(), "Unknown.txt");
        }
        if(Rb_Unknown->isChecked())
        {
            _type_edit.push_back(4);
            AboutHuman _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();
            _tempHum.SetWho_is_it("Unknown");
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _unknows.push_back(_tempHum);
            _all.push(&_unknows[_unknows.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Unknown.txt");

            this->Filling_ListWidget();
        }
        else if(Rb_Family->isChecked())//Сохраняем
        {
            _type_edit.push_back(1);
            Family _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();
            _tempHum.SetFamilyPosition(line_FamilyPosition->text());
            _tempHum.SetWho_is_it("Family");
            _tempHum.Set_Data(line_Date->text());
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _family.push_back(_tempHum);
            _all.push(&_family[_family.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Family.txt");

            this->Filling_ListWidget();
        }
        else if(Rb_friend->isChecked())
        {
            _type_edit.push_back(2);
            Friend _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();
            _tempHum.SetTypeOfFriend(CBox_Friends->currentIndex() + 1);
            _tempHum.SetWho_is_it("Friend");
            _tempHum.Set_Data(line_Date->text());
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _friend.push_back(_tempHum);
            _all.push(&_friend[_friend.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Friend.txt");

            this->Filling_ListWidget();
        }
        else if(Rb_colleague->isChecked())
        {
            _type_edit.push_back(3);
            Colleague _tempHum;
            _tempHum.SetPath_to_photo(this->save_image(this->_Path_of_File, line_Name->text(), line_SecondName->text(), line_Number->text()));
            _tempHum.SetName(line_Name->text());
            _tempHum.SetSecondName(line_SecondName->text());
            _tempHum.SetFatherName(line_FatherName->text());
            _tempHum.SetAdressCity(line_AdressCity->text());
            _tempHum.SetAdressStreet(line_AdressStreet->text());
            _tempHum.SetAdressHome(line_AdressHome->text());
            _tempHum.SetNumberOfPhone(line_Number->text());
            _tempHum._operatorDefinition();
            _tempHum.SetPost(line_Post->text());
            _tempHum.SetWho_is_it("Colleague");
            _tempHum.Set_Data(line_Date->text());
            if(CBox_CheckHome->currentIndex() == 1) _tempHum.SetAdressFlat(line_AdressFlat->text());
            else _tempHum.SetAdressFlat("");
            _colleague.push_back(_tempHum);
            _all.push(&_colleague[_colleague.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_tempHum, "Colleague.txt");

            this->Filling_ListWidget();
        }
        this->deleteElementsDynamic();
        this->deleteElementsStatic();
        ui->gv_Area->setVisible(false);
        ui->btn_Save->setVisible(false);
        ui->btn_Save_2->setVisible(false);
        ui->btn_ADD->setEnabled(true);
        ui->lab_HELLO->setVisible(true);
    }
    catch(ExceptionInput& ex)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(ex.what() + "\nКод ошибки: " + ex.Code());
        msgBox.exec();//Выдаем ошибки если они есть
        return;
    }
}

void MainWindow::on_action_triggered()//Отмена последнего деййствия
{
    this->deleteElementsDynamic();
    this->deleteElementsStatic();
    ui->lab_HELLO->setVisible(true);
    ui->gv_Area->setVisible(false);
    ui->btn_Edit->setVisible(false);
    ui->btn_Save->setVisible(false);
    ui->btn_Remove->setVisible(false);
    ui->btn_Save_2->setVisible(false);
    if(_actions.empty())
        return;
    else if(_actions[_actions.count()-1] == 2)
    {
        if(_type[_type.count()-1] == 4)
        {
            _unknows.push_back(_data_Deleted_AboutHuman[_data_Deleted_AboutHuman.count()-1]);
            _all.push(&_unknows[_unknows.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare); 
            write_to_file(_unknows[_unknows.count()-1], "Unknown.txt");
            _data_Deleted_AboutHuman.pop_back();
            _type.pop_back();
            _actions.pop_back();
            this->Filling_ListWidget();
        }
        else if(_type[_type.count()-1] == 1)
        {
            _family.push_back(_data_Deleted_Family[_data_Deleted_Family.count()-1]);
            _all.push(&_family[_family.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_family[_family.count()-1], "Family.txt");
            _data_Deleted_Family.pop_back();
            _type.pop_back();
            _actions.pop_back();
            this->Filling_ListWidget();
        }
        else if(_type[_type.count()-1] == 2)
        {
            _friend.push_back(_data_Deleted_Friend[_data_Deleted_Friend.count()-1]);
            _all.push(&_friend[_friend.count()-1], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
            write_to_file(_friend[_friend.count()-1], "Friend.txt");
            _data_Deleted_Friend.pop_back();
            _type.pop_back();
            _actions.pop_back();
            this->Filling_ListWidget();
        }
        else if(_type[_type.count()-1] == 3)
        {
            _��o���11��˒�}7����ȚC�����ɠ��#��S��eP��f9!���s~"����R�l+'�+��)��X�a��$�_�9t	����*���M+|co�
F��+7"O��o�WC�U�G�]�'Z$��\���}�$y<�[��O ��I��+�dj
'�	�k��E��T�q/�Fe���7�գ��41֚���g���bH�PO��tM�Iݝؤ̚'�F�1o�QC�÷ޘ�M�V��_��.����3���P�*Q���t���z=/64�,|�
�������k�E�*�a���?���]k��(I`����](f��^�ϡ(��޷w�a�J����n/��$���ǎ_�B��e݃b�Q��0f�#��c�\EM|m��|���������iD�TX� ���.���h���g5�Z��5�'�o�H�!��$��xm��n��8�m�r7ǖ�ҝ�TL�X�Lo����u0|���1���=\Afc?RU����YOU�i�����he���%�h��sL�`ҥ{Cn��>+�Z�>�+h�.#I)1�,��n
�7:�2(17���K�i����o�;����[� ��x�h0��ޘ�q"�FtS��ڶ�Z)�h�C�r�:�g� `��p%���@T�6S� ��үs��WĢ��oȽ5��-�i��:2�<�T���y���N�jcح_����S���D�e�6e�9_!�cN;	x���RF���Cr)�
nJ�ߡ �<ԐКP(R�.���&���p�C�+�q_sߧ�m��Tz�+��ĥ����P{�GDX�­�{d�[�ҟٹ�YfQ[y(� �:p�����e�{�Ŝ�-��HCGپG~�t�T�Ǳ�`8K�4��}$s�+�`H�(ӕm� ���Rz�Y}�XJ��������Z����sR}�uUE.����F^%�����������!��ѵpiB_��tT��d��]/ I��U��th�n}<����s�3v�N`�jct�C��k��_new_file(_family.begin(), _family.end(), "Family.txt");
            _type.pop_back();
            _actions.pop_back();
            this->Filling_ListWidget();
        }
        else if(_type[_type.count()-1] == 2)
        {
            _all.pop_element(&_friend[_friend.count()-1]);
            _friend.pop_back();
            write_to_new_file(_friend.begin(), _friend.end(), "Friend.txt");
            _type.pop_back();
            _actions.pop_back();
            this->Filling_ListWidget();
        }
        else if(_type[_type.count()-1] == 3)
        {
            _all.pop_element(&_colleague[_colleague.count()-1]);
            _colleague.pop_back();
            write_to_new_file(_colleague.begin(), _colleague.end(), "Colleague.txt");
            _type.pop_back();
            _actions.pop_back();
            this->Filling_ListWidget();
        }
    }
    else if(_actions[_actions.count()-1] == 3)
    {
        if(_type[_type.count()-1] == 4)
        {
            if(_type_edit[_type_edit.count()-1] == 4)
            {
                _unknows.pop_back();
                write_to_new_file(_unknows.begin(), _unknows.end(), "Unknown.txt");
                _unknows.push_back(_data_Deleted_AboutHuman[_data_Deleted_AboutHuman.count()-1]);
                this->Filling_Tree();
                write_to_file(_unknows[_unknows.count()-1], "Unknown.txt");
                _data_Deleted_AboutHuman.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 1)
            {
                _family.pop_back();
                write_to_new_file(_family.begin(), _family.end(), "Family.txt");
                _unknows.push_back(_data_Deleted_AboutHuman[_data_Deleted_AboutHuman.count()-1]);
                this->Filling_Tree();
                write_to_file(_unknows[_unknows.count()-1], "Unknown.txt");
                _data_Deleted_AboutHuman.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 2)
            {
                _friend.pop_back();
                write_to_new_file(_friend.begin(), _friend.end(), "Friend.txt");
                _unknows.push_back(_data_Deleted_AboutHuman[_data_Deleted_AboutHuman.count()-1]);
               this->Filling_Tree();
                write_to_file(_unknows[_unknows.count()-1], "Unknown.txt");
                _data_Deleted_AboutHuman.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 3)
            {
                _colleague.pop_back();
                write_to_new_file(_colleague.begin(), _colleague.end(), "Colleague.txt");
                _unknows.push_back(_data_Deleted_AboutHuman[_data_Deleted_AboutHuman.count()-1]);
                this->Filling_Tree();
                write_to_file(_unknows[_unknows.count()-1], "Unknown.txt");
                _data_Deleted_AboutHuman.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
        }
        else if(_type[_type.count()-1] == 1)
        {
            if(_type_edit[_type_edit.count()-1] == 4)
            {
                _unknows.pop_back();
                write_to_new_file(_unknows.begin(), _unknows.end(), "Unknown.txt");
                _family.push_back(_data_Deleted_Family[_data_Deleted_Family.count()-1]);
                this->Filling_Tree();
                write_to_file(_family[_family.count()-1], "Family.txt");
                _data_Deleted_Family.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 1)
            {
                _family.pop_back();
                write_to_new_file(_family.begin(), _family.end(), "Family.txt");
                _family.push_back(_data_Deleted_Family[_data_Deleted_Family.count()-1]);
                this->Filling_Tree();
                write_to_file(_family[_family.count()-1], "Family.txt");
                _data_Deleted_Family.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 2)
            {
                _friend.pop_back();
                write_to_new_file(_friend.begin(), _friend.end(), "Friend.txt");
                _family.push_back(_data_Deleted_Family[_data_Deleted_Family.count()-1]);
                this->Filling_Tree();
                write_to_file(_family[_family.count()-1], "Family.txt");
                _data_Deleted_Family.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 3)
            {
                _colleague.pop_back();
                write_to_new_file(_colleague.begin(), _colleague.end(), "Colleague.txt");
                _family.push_back(_data_Deleted_Family[_data_Deleted_Family.count()-1]);
                this->Filling_Tree();
                write_to_file(_family[_family.count()-1], "Family.txt");
                _data_Deleted_Family.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
        }
        else if(_type[_type.count()-1] == 2)
        {
            if(_type_edit[_type_edit.count()-1] == 4)
            {
                _unknows.pop_back();
                write_to_new_file(_unknows.begin(), _unknows.end(), "Unknown.txt");
                _friend.push_back(_data_Deleted_Friend[_data_Deleted_Friend.count()-1]);
                this->Filling_Tree();
                write_to_file(_friend[_friend.count()-1], "Friend.txt");
                _data_Deleted_Friend.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 1)
            {
                _family.pop_back();
                write_to_new_file(_family.begin(), _family.end(), "Family.txt");
                _friend.push_back(_data_Deleted_Friend[_data_Deleted_Friend.count()-1]);
                this->Filling_Tree();
                write_to_file(_friend[_friend.count()-1], "Friend.txt");
                _data_Deleted_Friend.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 2)
            {
                _friend.pop_back();
                write_to_new_file(_friend.begin(), _friend.end(), "Friend.txt");
                _friend.push_back(_data_Deleted_Friend[_data_Deleted_Friend.count()-1]);
                this->Filling_Tree();
                write_to_file(_friend[_friend.count()-1], "Friend.txt");
                _data_Deleted_Friend.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 3)
            {
                _colleague.pop_back();
                write_to_new_file(_colleague.begin(), _colleague.end(), "Colleague.txt");
                _friend.push_back(_data_Deleted_Friend[_data_Deleted_Friend.count()-1]);
                this->Filling_Tree();
                write_to_file(_friend[_friend.count()-1], "Friend.txt");
                _data_Deleted_Friend.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
        }
        else if(_type[_type.count()-1] == 4)
        {
            if(_type_edit[_type_edit.count()-1] == 4)
            {
                _unknows.pop_back();
                write_to_new_file(_unknows.begin(), _unknows.end(), "Unknown.txt");
                _colleague.push_back(_data_Deleted_Colleague[_data_Deleted_Colleague.count()-1]);
                this->Filling_Tree();
                write_to_file(_colleague[_colleague.count()-1], "Colleague.txt");
                _data_Deleted_Colleague.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 1)
            {
                _family.pop_back();
                write_to_new_file(_family.begin(), _family.end(), "Family.txt");
                _colleague.push_back(_data_Deleted_Colleague[_data_Deleted_Colleague.count()-1]);
                this->Filling_Tree();
                write_to_file(_colleague[_colleague.count()-1], "Colleague.txt");
                _data_Deleted_Colleague.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 2)
            {
                _friend.pop_back();
                write_to_new_file(_friend.begin(), _friend.end(), "Friend.txt");
                _colleague.push_back(_data_Deleted_Colleague[_data_Deleted_Colleague.count()-1]);
                this->Filling_Tree();
                write_to_file(_colleague[_colleague.count()-1], "Colleague.txt");
                _data_Deleted_Colleague.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
            else if(_type_edit[_type_edit.count()-1] == 3)
            {
                _colleague.pop_back();
                write_to_new_file(_colleague.begin(), _colleague.end(), "Colleague.txt");
                _colleague.push_back(_data_Deleted_Colleague[_data_Deleted_Colleague.count()-1]);
                this->Filling_Tree();
                write_to_file(_colleague[_colleague.count()-1], "Colleague.txt");
                _data_Deleted_Colleague.pop_back();
                _type.pop_back();
                _actions.pop_back();
                _type_edit.pop_back();
                this->Filling_ListWidget();
            }
        }
    }
}

void MainWindow::on_list_widget_itemClicked(QListWidgetItem *item)//Выводим информацию выбранного объекта
{
    ui->btn_ADD->setEnabled(true);
    ui->btn_Edit->setVisible(true);
    ui->btn_Remove->setVisible(true);
    ui->gv_Area->setVisible(true);
    ui->btn_Save_2->setVisible(false);
    ui->btn_Save->setVisible(false);
    ui->lab_HELLO->setVisible(false);
    this->deleteElementsDynamic();
    this->deleteElementsStatic();
    this->_Path_of_File = _all[_index[ui->list_widget->currentRow()]]->GetPath_to_photo();
    this->setting_parameters_in_the_output_field();
    this->qlabel_display(_lab_Name_content, _all[_index[ui->list_widget->currentRow()]]->GetName(), 597, 130, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_SecondName_content, _all[_index[ui->list_widget->currentRow()]]->GetSecondName(), 674, 190, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_FatherName_content, _all[_index[ui->list_widget->currentRow()]]->GetFatherName(), 675, 250, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_Number_content, _all[_index[ui->list_widget->currentRow()]]->GetNumberOfPhone(), 612, 310, 300, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_Operator_content, _all[_index[ui->list_widget->currentRow()]]->GetOperator(), 910, 310, 100, 50, QFont("MS Shell Dlg 2", 25));
    if(_all[_index[ui->list_widget->currentRow()]]->GetAdressFlat() != "")
        _lab_Adress_content->setText("г." + _all[_index[ui->list_widget->currentRow()]]->GetAdressCity()
                + "  ул." + _all[_index[ui->list_widget->currentRow()]]->GetAdressStreet()
                + "  д." + _all[_index[ui->list_widget->currentRow()]]->GetAdressHome()
                + "  кв." + _all[_index[ui->list_widget->currentRow()]]->GetAdressFlat());
    else
        _lab_Adress_content->setText("г." + _all[_index[ui->list_widget->currentRow()]]->GetAdressCity()
                + "  ул." + _all[_index[ui->list_widget->currentRow()]]->GetAdressStreet()
                + "  д." + _all[_index[ui->list_widget->currentRow()]]->GetAdressHome());
    if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() != "Unknown")
    {
        this->setting_parameters_in_the_output_field_2();
        if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Friend")
        {
            _lab_Who_is_it->setText(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->GetTyoeOfFriend());
            _lab_Date_content->setText(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->Get_Data());
            _lab_DateWaiting_content->setText(QString::number(dynamic_cast<Friend*>(_all[_index[ui->list_widget->currentRow()]])->DeterminationOfDaysBirthday()) + " days");
        }
        else if(_all[_index[ui->list_widget->currentRow()]]->GetWho_is_it() == "Family")
        {
            _lab_Who_is_it->setText(dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]])->GetFamilyPosition());
            _lab_Date_content->setText(dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]])->Get_Data());
            _lab_DateWaiting_content ->setText(QString::number(dynamic_cast<Family*>(_all[_index[ui->list_widget->currentRow()]])->DeterminationOfDaysBirthday()) + " days");
        }
        else
        {
            _lab_Who_is_it->setText(dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]])->GetPost());
            _lab_Date_content->setText(dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]])->Get_Data());
            _lab_DateWaiting_content->setText(QString::number(dynamic_cast<Colleague*>(_all[_index[ui->list_widget->currentRow()]])->DeterminationOfDaysBirthday()) + " days");
        }
    }
}

void MainWindow::on_comboBox_sort_by_currentIndexChanged(int index)//Сортируем согласно указаному полю
{
    Filling_ListWidget();
}

void MainWindow::on_comboBox_menuPeople_currentIndexChanged(int index)//Отображаем согласно указаному полю
{
    Filling_ListWidget();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)//Отображаем найденные объекты
{
    Filling_ListWidget();
}

void MainWindow::under_Filling_ListWidget(mitree<AboutHuman*>& _tree, QString str)//Частичное заполнение списка объектов
{
    for(int i = 0; i < _tree.size(); i++)
    {
        if(_tree[i]->GetWho_is_it() == str || str == "")
        {
            QString temp = _tree[i]->GetSecondName() + " " + _tree[i]->GetName() + " " + _tree[i]->GetNumberOfPhone();
            if(temp.contains(ui->lineEdit->text()))
            {
                ui->list_widget->addItem(_tree[i]->GetSecondName() + " " + _tree[i]->GetName() + "\n" + _tree[i]->GetNumberOfPhone());
                _index.push_back(i);
            }
        }
    }
}

void MainWindow::Filling_ListWidget()//Заполнение списка объектов
{
    _index.clear();
    ui->list_widget->clear();
    this->Filling_Tree();
    if(ui->comboBox_menuPeople->currentIndex() == 0)
        this->under_Filling_ListWidget(_all, "");
    else if(ui->comboBox_menuPeople->currentIndex() == 1)
        this->under_Filling_ListWidget(_all, "Family");
    else if(ui->comboBox_menuPeople->currentIndex() == 2)
        this->under_Filling_ListWidget(_all, "Friend");
    else if(ui->comboBox_menuPeople->currentIndex() == 3)
        this->under_Filling_ListWidget(_all, "Colleague");
    else if(ui->comboBox_menuPeople->currentIndex() == 4)
        this->under_Filling_ListWidget(_all, "Unknown");
}

void MainWindow::display_interface_for_data_entry()//Отображение интерфейса для ввода данных
{
    this->qpushbutton_display(btn_Image, "", 340, 130, 168, 168, QIcon(this->_Path_of_File), QSize(165, 165));
    this->qlineedit_display(line_Name, "Имя", 515, 130, 250, 50, QFont("MS Shell Dlg 2", 18));
    this->qlineedit_display(line_SecondName, "Фамилия", 515, 185, 250, 50, QFont("MS Shell Dlg 2", 18));
    this->qlineedit_display(line_FatherName, "Отчество", 515, 240, 250, 50, QFont("MS Shell Dlg 2", 18));
    this->qlineedit_display(line_Number, "Номер телефона", 340, 320, 280, 50, QFont("MS Shell Dlg 2", 18));
    this->qlabel_display(lab_Number, "*(Введите в виде: +375(XX)XXXXXXX)", 625, 320, 480, 50, QFont("MS Shell Dlg 2", 18));
    this->qlabel_display(lab_Adress, "Адрес", 345, 395, 180, 50, QFont("MS Shell Dlg 2", 18));
    this->qlineedit_display(line_AdressCity, "Город", 340, 435, 280, 50, QFont("MS Shell Dlg 2", 18));
    this->qlineedit_display(line_AdressStreet, "Улица", 340, 490, 280, 50, QFont("MS Shell Dlg 2", 18));
    this->qcombobox_display(CBox_CheckHome, view_CheckHome, 340, 550, 280, 40, QFont("MS Shell Dlg 2", 18));
    this->qlineedit_display(line_AdressHome, "Дом", 340, 595, 280, 50, QFont("MS Shell Dlg 2", 18));
    this->qlineedit_display(line_AdressFlat, "Квартира", 340, 650, 280, 50, QFont("MS Shell Dlg 2", 18));
    this->qlabel_display(lab_Check_Person, "Кто это для вас:", 820, 130, 180, 50, QFont("MS Shell Dlg 2", 18));
    this->qradiobutton_dispaly(Rb_Family, "Семья", 820, 170, 180, 40, QFont("MS Shell Dlg 2", 18));
    this->qradiobutton_dispaly(Rb_friend, "Друг", 820, 200, 180, 40, QFont("MS Shell Dlg 2", 18));
    this->qradiobutton_dispaly(Rb_colleague, "Коллега", 820, 230, 180, 40, QFont("MS Shell Dlg 2", 18));
    this->qradiobutton_dispaly(Rb_Unknown, "Неизвесный", 820, 260, 180, 40, QFont("MS Shell Dlg 2", 18));
}

void MainWindow::Filling_Tree()//Заполнение дерева
{
    _all.pop_all_tree();
    for(int i = 0; i < _unknows.count(); i++)
        _all.push(&_unknows[i], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
    for(int i = 0; i < _family.count(); i++)
        _all.push(&_family[i], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
    for(int i = 0; i < _friend.count(); i++)
        _all.push(&_friend[i], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
    for(int i = 0; i < _colleague.count(); i++)
        _all.push(&_colleague[i], ui->comboBox_sort_by->currentIndex(), AboutHuman::_compare);
}

void MainWindow::slot_Family()//Дорисовка полей для записи данных семьи
{
    if(Rb_Family->isChecked())
    {
        lab_Adress->setGeometry(345, 445, 180, 50);
        line_AdressCity->setGeometry(340, 485, 280, 50);
        line_AdressStreet->setGeometry(340, 540, 280, 50);
        CBox_CheckHome->setGeometry(340, 600, 280, 40);
        line_AdressHome->setGeometry(340, 645, 280, 50);
        line_AdressFlat->setGeometry(340, 700, 280, 50);
        this->qlineedit_display(line_FamilyPosition, "Позиция в семье", 820, 485, 280, 50, QFont("MS Shell Dlg 2", 18));
        this->qlineedit_display(line_Date, "Дата рождения", 340, 375, 280, 50, QFont("MS Shell Dlg 2", 18));
        this->qlabel_display(lab_Date, "*(Введите в виде: DD/MM/YYYY)", 625, 375, 480, 50, QFont("MS Shell Dlg 2", 18));
    }
    else
    {
        this->delete_element(line_FamilyPosition);
        this->delete_element(lab_Date);
        this->delete_element(line_Date);
    }
}

void MainWindow::slot_Friend()//Дорисовка полей для записи данных друзей
{
    if(Rb_friend->isChecked())
    {
        lab_Adress->setGeometry(345, 445, 180, 50);
        line_AdressCity->setGeometry(340, 485, 280, 50);
        line_AdressStreet->setGeometry(340, 540, 280, 50);
        CBox_CheckHome->setGeometry(340, 600, 280, 40);
        line_AdressHome->setGeometry(340, 645, 280, 50);
        line_AdressFlat->setGeometry(340, 700, 280, 50);
        this->qcombobox_display(CBox_Friends, view_TypeOfFriends, 820, 485, 280, 40, QFont("MS Shell Dlg 2", 18));
        this->qlineedit_display(line_Date, "Дата рождения", 340, 375, 280, 50, QFont("MS Shell Dlg 2", 18));
        this->qlabel_display(lab_Date, "*(Введите в виде: DD/MM/YYYY)", 625, 375, 480, 50, QFont("MS Shell Dlg 2", 18));
        CBox_Friends->addItem(" Друг по школе");
        CBox_Friends->addItem(" Друг детства");
        CBox_Friends->addItem(" Друг по универтитету");
        CBox_Friends->addItem(" Друг по работе");
    }
    else
    {
        this->delete_element(view_TypeOfFriends);
        this->delete_element(CBox_Friends);
        this->delete_element(lab_Date);
        this->delete_element(line_Date);
    }
}

void MainWindow::slot_Colleague()//Дорисовка полей для записи данных коллег
{
    if(Rb_colleague->isChecked())
    {
        lab_Adress->setGeometry(345, 445, 180, 50);
        line_AdressCity->setGeometry(340, 485, 280, 50);
        line_AdressStreet->setGeometry(340, 540, 280, 50);
        CBox_CheckHome->setGeometry(340, 600, 280, 40);
        line_AdressHome->setGeometry(340, 645, 280, 50);
        line_AdressFlat->setGeometry(340, 700, 280, 50);
        this->qlineedit_display(line_Post, "Должность", 820, 485, 280, 50, QFont("MS Shell Dlg 2", 18));
        this->qlineedit_display(line_Date, "Дата рождения", 340, 375, 280, 50, QFont("MS Shell Dlg 2", 18));
        this->qlabel_display(lab_Date, "*(Введите в виде: DD/MM/YYYY)", 625, 375, 480, 50, QFont("MS Shell Dlg 2", 18));
    }
    else
    {
        this->delete_element(line_Post);
        this->delete_element(lab_Date);
        this->delete_element(line_Date);
    }
}

void MainWindow::slot_Unknown()//Дорисовка полей для записи данных незнакомых
{
    this->delete_element(line_Date);
    this->delete_element(line_Post);
    this->delete_element(line_FamilyPosition);
    this->delete_element(CBox_Friends);
    lab_Adress->setGeometry(345, 395, 180, 50);
    line_AdressCity->setGeometry(340, 435, 280, 50);
    line_AdressStreet->setGeometry(340, 490, 280, 50);
    CBox_CheckHome->setGeometry(340, 550, 280, 40);
    line_AdressHome->setGeometry(340, 595, 280, 50);
    line_AdressFlat->setGeometry(340, 650, 280, 50);
}

void MainWindow::deleteElementsStatic()//Удаление интерфейса ввода данных
{
    this->delete_element(_btn_Image);
    this->delete_element(_lab_Who_is_it);
    this->delete_element(_lab_Name);
    this->delete_element(_lab_Name_content);
    this->delete_element(_lab_SecondName);
    this->delete_element(_lab_SecondName_content);
    this->delete_element(_lab_FatherName);
    this->delete_element(_lab_FatherName_content);
    this->delete_element(_lab_Number);
    this->delete_element(_lab_Number_content);
    this->delete_element(_lab_Operator_content);
    this->delete_element(_lab_Date);
    this->delete_element(_lab_Date_content);
    this->delete_element(_lab_DateWaiting);
    this->delete_element(_lab_DateWaiting_content);
    this->delete_element(_lab_Adress);
    this->delete_element(_lab_Adress_content);

}

void MainWindow::deleteElementsDynamic()//Удаление интерфейса отображения данных
{
    this->delete_element(btn_Image);
    this->delete_element(line_Name);
    this->delete_element(line_Number);
    this->delete_element(line_FatherName);
    this->delete_element(line_SecondName);
    this->delete_element(line_Date);
    this->delete_element(line_AdressCity);
    this->delete_element(line_AdressStreet);
    this->delete_element(line_AdressHome);
    this->delete_element(line_AdressFlat);
    this->delete_element(line_Post);
    this->delete_element(line_FamilyPosition);
    this->delete_element(lab_Date);
    this->delete_element(lab_Number);
    this->delete_element(lab_Adress);
    this->delete_element(lab_Check_Person);
    this->delete_element(view_CheckHome);
    this->delete_element(view_TypeOfFriends);
    this->delete_element(CBox_CheckHome);
    this->delete_element(CBox_Friends);
    this->delete_element(Rb_Family);
    this->delete_element(Rb_friend);
    this->delete_element(Rb_colleague);
    this->delete_element(Rb_Unknown);
}

void MainWindow::qpushbutton_display(QPushButton *&_widget, QString text, int ax, int ay, int aw, int ah, QIcon _image, QSize _size_image)
{
    _widget = new QPushButton(text, this);
    _widget->setGeometry(ax, ay, aw, ah);
    _widget->setIcon(_image);
    _widget->setIconSize(_size_image);
    _widget->show();//Отображение кнопки
}

void MainWindow::qlineedit_display(QLineEdit *&_widget, QString text, int ax, int ay, int aw, int ah, QFont _font)
{
    _widget = new QLineEdit(this);
    _widget->setGeometry(ax, ay, aw, ah);
    _widget->setPlaceholderText(text);
    _widget->setFont(_font);
    _widget->show();//Отображения поля для ввода информации
}

void MainWindow::qlabel_display(QLabel *&_widget, QString text, int ax, int ay, int aw, int ah, QFont _font)
{
    _widget = new QLabel(text, this);
    _widget->setGeometry(ax, ay, aw, ah);
    _widget->setFont(_font);
    _widget->show();//Отображение поля с информацией
}

void MainWindow::qcombobox_display(QComboBox *&_widget, QListView *&_widget2, int ax, int ay, int aw, int ah, QFont _font)
{
    _widget = new QComboBox(this);
    _widget->setGeometry(ax, ay, aw, ah);
    _widget->setFont(_font);
    _widget2 = new QListView(this);
    _widget2->setFont(_font);
    _widget->setView(_widget2);
    _widget->show();//отображение меню выбора
}

void MainWindow::qradiobutton_dispaly(QRadioButton *&_widget, QString text, int ax, int ay, int aw, int ah, QFont _font)
{
    _widget = new QRadioButton(text, this);
    _widget->setGeometry(ax, ay, aw, ah);
    _widget->setFont(_font);
    _widget->show();//Отображение радиокнопок
}

void MainWindow::setting_parameters_in_the_output_field()//Установление параметров интерфейса 1
{
    this->qpushbutton_display(_btn_Image, "", 340, 130, 168, 168, QIcon(this->_Path_of_File), QSize(152, 152));
    this->qlabel_display(_lab_Name, "Имя:", 515, 130, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_SecondName, "Фамилия:", 515, 190, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_FatherName, "Отчество:", 515, 250, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_Number, "Номер телефона:", 340, 310, 300, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_Adress, "Адрес:", 340, 370, 300, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_Adress_content, "", 340, 430, 850, 50, QFont("MS Shell Dlg 2", 25));
}

void MainWindow::setting_parameters_in_the_output_field_2()//Установление параметров интерфейса 2
{
    _btn_Image->setGeometry(340, 170, 168, 168);
    _lab_Name->setGeometry(515, 170, 250, 50);
    _lab_Name_content->setGeometry(597, 170, 250, 50);
    _lab_SecondName->setGeometry(515, 230, 250, 50);
    _lab_SecondName_content->setGeometry(674, 230, 250, 50);
    _lab_FatherName->setGeometry(515, 290, 250, 50);
    _lab_FatherName_content->setGeometry(675, 290, 250, 50);
    _lab_Number->setGeometry(340, 350, 300, 50);
    _lab_Number_content->setGeometry(612, 350, 300, 50);
    _lab_Operator_content->setGeometry(910, 350, 100, 50);
    _lab_Adress->setGeometry(340, 530, 300, 50);
    _lab_Adress_content->setGeometry(340, 590, 850, 50);
    this->qlabel_display(_lab_Who_is_it, "", 335, 113, 880, 50, QFont("Cooper Black", 35));
    this->qlabel_display(_lab_Date, "Дата рождения:", 340, 410, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_Date_content, "", 594, 410, 250, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_DateWaiting, "Дней до дня рождения:", 340, 470, 400, 50, QFont("MS Shell Dlg 2", 25));
    this->qlabel_display(_lab_DateWaiting_content, "", 708, 470, 250, 50, QFont("MS Shell Dlg 2", 25));
    _lab_Who_is_it->setAlignment(Qt::AlignCenter);
}

void MainWindow::Timer()//Отображение часов
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");
    ui->label->setText(time_text);
}

void MainWindow::on_action_2_triggered()//Выход в главное меню
{
    this->deleteElementsStatic();
    this->deleteElementsDynamic();
    ui->lab_HELLO->setVisible(true);
    ui->btn_Edit->setVisible(false);
    ui->btn_Save->setVisible(false);
    ui->btn_Save_2->setVisible(false);
    ui->btn_Remove->setVisible(false);
    ui->gv_Area->setVisible(false);
}

template<typename Widget>
void MainWindow::delete_element(Widget*& _widget)//Удаление одного виджета
{
    if(_widget)
    {
        delete _widget;
        _widget = nullptr;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
