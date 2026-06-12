#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QObject>
#include <memory>
#include <QGridLayout>
#include <iostream>
#include <optional>


struct math_quation
{
    std::list<QString> list;
    int left_brackets_count = 0;
    int right_brackets_count = 0;

    std::list<int> l_brackets_pos;
    std::list<int> r_brackets_pos;
};

struct range_quation
{
    math_quation qutiation;
    std::list<QString>::iterator begin;
    std::list<QString>::iterator end;
};

class main_window : public QWidget
{
    Q_OBJECT

public:

    void create_buttns()
    {
        //create numbers
        for(int y = 0; y < 3; y++)
        {
            for(int x = 0; x < 3; x++)
            {
            numbers[y][x]  = new QPushButton(std::to_string(3*y+x+1).c_str(), this);
            numbers[y][x]->setFixedSize(80,80);
            }
        }

        //create zero
        zero = new QPushButton("0");

        //create fucntions
        buttons[0] = new QPushButton("+", this);
        buttons[1] = new QPushButton("-", this);
        buttons[2] = new QPushButton("*", this);
        buttons[3] = new QPushButton("/", this);
        
        for (int i = 0; i < 4; i++)
        {
            buttons[i]->setFixedSize(80, 80);
        }

        //create quael
        quael = new QPushButton("=", this);
        quael->setFixedSize(80, 80);

        //create delete
        delete_butt = new QPushButton("<", this);
        delete_butt->setFixedSize(80,80);


        //create label
        label = new QLabel("0", this);
        label->setMaximumSize(400, 120);
        label->setMinimumSize(200, 60);

        //create brackets
        bracket_left = new QPushButton("(", this);
        bracket_left->setFixedSize(80, 80);

        bracket_right = new QPushButton(")", this);
        bracket_right->setFixedSize(80, 80);
        

        grid = new QGridLayout(this);
    }

    void set_buttns()
    {
        //set numbers
        for(int y = 0; y < 3; y++)
        {
            for(int x = 0; x < 3; x++)
            {
                grid->addWidget(numbers[y][x], 1+y, x);
            }
        }

        //set zero
        zero->setFixedSize(80,80);
        grid->addWidget(zero, 4, 1);

        //set functions
        for(int y = 0; y < 4; y++)
        {
            grid->addWidget(buttons[y], y+1, 4);
        }


        //set quael
        grid->addWidget(quael, 4, 0);
        grid->addWidget(label, 0, 0);


        //set delete
        grid->addWidget(delete_butt, 4, 2);

        grid->addWidget(bracket_left, 5, 0);
        grid->addWidget(bracket_right, 5, 1);

        setLayout(grid);
    }

    void connect_buttns()
    {
        for(int y = 0; y < 3; y++)
        {
            for(int x = 0; x < 3; x++)
            {
               connect(numbers[y][x], &QPushButton::clicked, this, std::bind(&main_window::add_numb, this, numbers[y][x]->text()));
            }
        }
        connect(zero, &QPushButton::clicked, this, std::bind(&main_window::add_numb, this, zero->text()));
        
        for(int y = 0; y < 4; y++)
        {
            connect(buttons[y], &QPushButton::clicked, this, std::bind(&main_window::add_symb, this, buttons[y]->text()));
        }

        connect(quael, &QPushButton::clicked, this, std::bind(&main_window::answer_func, this, std::ref(all)));
        connect(delete_butt, &QPushButton::clicked, this, &main_window::delete_symb);

        connect(bracket_left, &QPushButton::clicked, this, std::bind(&main_window::add_bracket, this, "("));
        connect(bracket_right, &QPushButton::clicked, this, std::bind(&main_window::add_bracket, this, ")"));
    }

    main_window()
    {
        setWindowTitle("Calculator");
        resize(500, 500);

        this->setFixedSize(340, 480);

        create_buttns();
        set_buttns();
        connect_buttns();
        
        //connect(minus_butt, &QPushButton::clicked, this, &main_window::minus);
    }

    
private slots:

    void add_numb(QString text)
    {
        math_str += text;
        label->setText(math_str);
    }

    bool can_minus(QString text)
    {
        if (math_str.size() >= 2)
        {
            if (QString("+-/*").contains(math_str[math_str.size()-1]) && text == '-' && math_str[math_str.size()-2].isDigit())
            {
                return true;
            }
        }
        else if (math_str.size() == 0)
        {
            return true;
        }
        
        return false;
    }

    void add_symb(QString text)
    {
        if (math_str.size() > 0)
        {
            if (math_str[math_str.size()-1].isDigit() || math_str[math_str.size()-1] == ")" || can_minus(text))
            {
                math_str += text;
                label->setText(math_str);
            }
        }
        else if(text == "-")
        {
            math_str += text;
            label->setText(math_str);
        }

    }

    void add_bracket(QString text)
    {

        if (text == "(")
        {
            all.left_brackets_count++;
            math_str += text;
            label->setText(math_str);
        }
        else if (text == ")" && all.left_brackets_count > all.right_brackets_count)
        {
            all.right_brackets_count++;
            math_str += text;
            label->setText(math_str);
        }
    }

    void delete_symb()
    {
        if (math_str.size() >= 1)
        {
            if (math_str[math_str.size()-1] == "(")
            {
                all.left_brackets_count--;
            }
            else if(math_str[math_str.size()-1] == ")")
            {
                all.right_brackets_count--;
            }

            math_str.resize(math_str.size()-1);
            label->setText(math_str);
        }
    }


    void add_number_to_list(QString &number, QString math_symb)
    {
        all.list.push_back(number);
        number = "";
        all.list.push_back(math_symb);
    }

    void get_list()
    {
        all.list.clear();
        QString all_text = label->text();
        bool add_minus = false;
        QString number;
        std::string numb_std;
        for (int i = 0; i < all_text.size(); i++)
        {
            
            if(all_text[i] == '-')
            {
                if (i == 0)
                {
                    number+=all_text[i];
                    add_minus = true;
                }
                else if(i >= 2)
                {
                    if (all_text[i-2].isDigit() && !all_text[i-1].isDigit())
                    {
                        number+=all_text[i];
                        add_minus = true;
                    }
                    else 
                    {
                        add_number_to_list(number, all_text[i]);
                    }
                }
                else 
                {
                    add_number_to_list(number, all_text[i]);
                }
            }
            else if(all_text[i].isDigit())
            {
                number+=all_text[i];
            }
            else
            {
                add_number_to_list(number, all_text[i]);
            }
            numb_std = number.toStdString();
        }
        all.list.push_back(number);
    }


    std::vector<std::string> get_std_from_qstr()
    {
        std::vector<std::string> std_str_vector;

        for (auto i = all.list.begin(); i != all.list.end(); i++)
        {
            std_str_vector.push_back((*i).toStdString());
        }
        
        return std_str_vector;
    }

    int get_numb(std::list<QString>::iterator &i)
    {
        int f_numb = (*std::prev(i, 1)).toInt();
        int s_numb = (*std::next(i, 1)).toInt();

        return 0;
    }

    bool math(std::list<QString>::iterator &i, char what_to_do, std::list<QString> &this_list)
    {
        bool has_job_func = false;
        if ((*i) == what_to_do)
        {

        int f_numb = (*std::prev(i, 1)).toInt();
        int s_numb = (*std::next(i, 1)).toInt();

        

        int answer;
        switch (what_to_do)
        {
        case '*':
            answer = f_numb * s_numb;
            break;

        case '/':
            answer = f_numb / s_numb;
            break;
        
        case '+':
            answer = f_numb + s_numb;
            break;

        case '-':
            answer = f_numb - s_numb;
            break;

        default:
            has_job_func = true;
            break;
        }

        auto ins_itt = this_list.erase(std::prev(i, 1), std::next(i, 2));
        i = this_list.insert(ins_itt, QString(std::to_string(answer).c_str()));
        }
        

        return has_job_func;
    }


    QString set_label(const math_quation work)
    {
        QString text_all;
        for(auto itt = work.list.begin(); itt != work.list.end(); itt++)
        {
            text_all += *itt;
        }
        return text_all;
    }

    // 1     1 2    2
    // ( 1+1 ) ( 1+1)

    //  1   2     1  2
    //  ( 1+( 1+1 ) )
    void insert(math_quation &work, std::list<QString>::iterator start, std::list<QString>::iterator end, const math_quation new_insert)
    {
        auto pos = work.list.erase(start, end);
        work.list.insert(pos, new_insert.list.begin(), new_insert.list.end());
    }

    //  1 2     1   2
    //  ( ( 1+1 )+1 )

    //        1      0  1      0
    //  1 + 2 ( 2 + 2)  ( 2 + 1) 
    void clear_quation(math_quation &work)
    {
        while(true)
        {
            auto empthy = std::find(work.list.begin(), work.list.end(), "");
            if (empthy != work.list.end())
            {
                work.list.erase(empthy);
            }
            else
            {
                break;
            }
        }
    }

    std::optional<range_quation> check_brackets(math_quation &work)
    {
        bool finded;
        auto start = std::find(work.list.begin(), work.list.end(), "(");
        auto end = std::next(start);
        int counter = 1;
        

        if (start == work.list.end())
        {
            return std::nullopt;
        }

        for(; counter != 0; end++)
        {
            if (*end == "(")
            {
                counter++;
            }
            else if (*end == ")")
            {
                counter--;
            }
        }

        if (counter == 0)
        {
            range_quation qwe;
            qwe.qutiation.list = std::list(std::next(start), std::prev(end));

            qwe.qutiation.left_brackets_count = work.left_brackets_count-1;
            qwe.qutiation.right_brackets_count = work.right_brackets_count-1;

            qwe.begin = start;
            qwe.end = end;

            std::optional<range_quation> ret = qwe;

            std::string text_all;
            text_all = set_label(ret.value().qutiation).toStdString();

            return ret;
        }

        return std::nullopt;
    }



    math_quation check_answer(math_quation &work)
    {
        bool has_job = true;
        
        std::string text_all;
        std::string text;
        text_all = set_label(work).toStdString();

        //check brackets
        if (work.left_brackets_count > 0)
        {
            bool has_brackets = true;
            while(has_brackets)
            {
                auto answer = check_brackets(work);
                if (answer.has_value())
                {
                    text_all = set_label(work).toStdString();

                    auto qutiation_answer = check_answer(answer.value().qutiation);

                    insert(work, answer.value().begin, answer.value().end, qutiation_answer);
                    clear_quation(work);

                    text_all = set_label(work).toStdString();
                }
                else{
                    has_brackets = false;
                }

            }
        }

        while(has_job)
        {
            

            has_job = false;
            int size = work.list.size();
            int ind = 0;
            for(auto i = work.list.begin(); i != work.list.end(); ++i)
            {
                text = (*i).toStdString();
                //text_all = set_label().toStdString();

                has_job = math(i, '*', work.list);
                text_all = set_label(work).toStdString();

                has_job = math(i, '/', work.list);
                text_all = set_label(work).toStdString();


                ind++;
            }
            
            text_all = set_label(work).toStdString();
            ind = 0;
            for(auto i = work.list.begin(); i != work.list.end(); ++i)
            {
                text = (*i).toStdString();


                has_job = math(i, '+', work.list);
                text_all = set_label(work).toStdString();

                has_job = math(i, '-', work.list);
                text_all = set_label(work).toStdString();
                
                ind++;
            }

            text_all = set_label(work).toStdString();

        }

        label->setText(*work.list.begin());
        math_str.clear();
        
        return work;
    }

    
    void answer_func(math_quation &work)
    {
        if (work.left_brackets_count == work.right_brackets_count)
        {
            get_list();
            std::vector<std::string> std_vect = get_std_from_qstr();
            auto answer = check_answer(work);
        }
        
    }



    

private:
    math_quation all;

    QString math_str;

    QPushButton *numbers[3][3];
    QPushButton *buttons[4];
    QPushButton *quael;
    QPushButton *delete_butt;
    QPushButton *zero;

    QPushButton *bracket_left;
    QPushButton *bracket_right;

    QLabel *label;
    QGridLayout *grid;

    std::vector<char> all_symbols_in_line;


};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    main_window window;

    window.show();
    

    return app.exec();
}

#include "main.moc"