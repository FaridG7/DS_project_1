#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


class node {
public:
	// type-> head = 0,company = 1,series brand = 2,model brand = 3,vehicle = 4
	int type;
	string N, A;
	int Y, M;
	node* next;
	node* down;
	node() { this->type = 0; this->next = NULL; this->down = NULL; }
	node(string name, string address, node* down) {
		this->type = 1; this->N = name;
		this->A = address; this->next = NULL; this->down = down;
	}
	node(string title, int type, node* down) {
		this->type = type; this->N = title;
		this->next = NULL; this->down = down;
	}
	node(string color, string licence, int year, int mileage) {
		this->type = 4; this->N = color; this->A = licence; this->Y = year;
		this->M = mileage; this->next = NULL; this->down = NULL;
	}
	void set_next(node* next) { this->next = next; }
	void set_down(node* down) { this->down = down; }
	node* get_next() { return next; }
	node* get_down() { return down; }
};
class database {
private:
	node* root;
public:
	database() { this->root = new node(); }
	~database() { Remove_the_hole_node(root); }
	node* Get_root() { return root; }
	node* Get_Cdown(string);
	node* Get_Bdown(node*, string);
	node* Get_Mdown(node*, string);
	bool Is_company_there(string);
	bool Is_brand_there(string, string);
	bool Is_model_there(string, string, string);
	bool Is_vehicle_there(string, string, string, string);
	void Insert_company(string, string);
	void Insert_brand(string, string);
	void Insert_model(string, string, string);
	void Insert_vehicle(string, string, string, int, string, string, int = 0);
	void Remove_company(string);
	void Remove_brand(string, string);
	void Remove_model(string, string, string);
	void Remove_vehicle(string, string, string, string);
	void Remove_the_hole_node(node*);
	void Clear_list();
	string Check_a_vehicle(string licence);
	void print_database();
	void print_a_c_of_database(string);
	void print_a_b_of_database(string, string);
	void print_a_m_of_database(string, string, string);
	void Vihecles_with_specified_color(string);
	void Vihecles_with_specified_year_produced(int);
	void Vihecles_with_specified_mileage(int, int);
};

void database::Insert_company(string name, string address) {
	if (Is_company_there(name)) return;
	node* new_hn = new node;
	node* new_node = new node(name, address, new_hn);
	if (!(root->next)) { this->root->next = new_node; return; };
	node* current = root->next;
	while (current->next)
		current = (current->next);
	current->next = new_node;
}
void database::Insert_brand(string name, string title) {
	if (!Is_company_there(name)) {
		cerr << "this company name doesn't exist" << "("<<name << "," << title 
			<< ")"<< endl;
		return;
	}
	if (Is_brand_there(name, title)) return;
	node* new_hn = new node;
	node* new_node = new node(title, 2, new_hn);
	node* x = Get_Cdown(name);
	if (!(x->next)) { x->next = new_node; return; };
	node* current = x->next;
	while (current->next)
		current = (current->next);
	current->next = new_node;
}
void database::Insert_model(string name, string title1, string title2) {
	if (!Is_company_there(name)) {
		cerr << "this company name doesn't exist" << "(" << name << "," << title1
			<< "," << title2 << ")" << endl;
		return;
	}
	if (!Is_brand_there(name, title1)) {
		cerr << "this brand title doesn't exist" << endl;
		return;
	}
	if (Is_model_there(name, title1, title2)) return;
	node* new_hn = new node;
	node* new_node = new node(title2, 3, new_hn);
	node* x1 = Get_Cdown(name);
	node* x2 = Get_Bdown(x1, title1);
	if (!(x2->next)) { x2->next = new_node; return; };
	node* current = x2->next;
	while (current->next)
		current = (current->next);
	current->next = new_node;
}
void database::Insert_vehicle(string name, string title1, string title2, int year,
	string color, string licence, int mileage) {
	if (!Is_company_there(name)) {
		cerr << "this company name doesn't exist" << "(" << name << "," << title1
			<< "," << title2 <<"," << licence << ")" << endl;
		return;
	}
	if (!Is_brand_there(name, title1)) {
		cerr << "this brand title doesn't exist" << endl;
		return;
	}
	if (!Is_model_there(name, title1, title2)) {
		cerr << "this model title doesn't exist" << endl;
		return;
	}
	if (Is_vehicle_there(name, title1, title2, licence)) return;
	node* new_node = new node(color, licence, year, mileage);
	node* x1 = Get_Cdown(name);
	node* x2 = Get_Bdown(x1, title1);
	node* x3 = Get_Mdown(x2, title2);
	if (!(x3->next)) { x3->next = new_node; return; };
	node* current = x3->next;
	while (current->next)
		current = (current->next);
	current->next = new_node;
};

void database::Remove_company(string name) {
	if (!Is_company_there(name)) return;
	node* c = root;
	while (c->next) {
		if (c->next->N == name)
			break;
		c = c->next;
	}
	node* c2 = c->next->next;
	node* b = c->next->down;
	Remove_the_hole_node(b);
	delete c->next;
	c->next = c2;
}
void database::Remove_brand(string name, string title) {
	if (!Is_company_there(name)) return;
	if (!Is_brand_there(name, title)) return;
	node* c = Get_Cdown(name);
	while (c->next) {
		if (c->next->N == title)
			break;
		c = c->next;
	}
	node* c2 = c->next->next;
	node* b = c->next->down;
	Remove_the_hole_node(b);
	delete c->next;
	c->next = c2;
	
}
void database::Remove_model(string name, string title1, string title2) {
	if (!Is_brand_there(name, title1)) return;
	node* c = Get_Bdown(Get_Cdown(name), title1);
	while (c->next) {
		if (c->next->N == title2)
			break;
		c = c->next;
	}
	node* c2 = c->next->next;
	node* b = c->next->down;
	Remove_the_hole_node(b);
	delete c->next;
	c->next = c2;
}
void database::Remove_vehicle(string name, string title1, string title2, string licence) {
	if (!Is_model_there(name, title1, title2)) return;
	node* c = Get_Mdown(Get_Bdown(Get_Cdown(name), title1), title2);
	while (c->next) {
		if (c->next->A == licence)
			break;
		c = c->next;
	}
	node* c2 = c->next->next;
	delete c->next;
	c->next = c2;
}

void database::Remove_the_hole_node(node* x) {
	if (!(x->next)) { delete x; return; }
	if (x->next->type < 4) {
		node* r = x->next;
		node* r2 = r->next;
		while (r2) {
			Remove_the_hole_node(r->down);
			delete r;
			r = r2;
			r2 = r2->next;
		}
		Remove_the_hole_node(r->down);
		delete r;
	}
	else {
		node* r = x->next;
		node* r2 = r->next;
		while (r2) {
			delete r;
			r = r2;
			r2 = r2->next;
		}
		delete r;
	}
	delete x;
}

bool database::Is_company_there(string name) {
	node* current = root->next;
	if (!current) return false;
	while (current) {
		if (current->N == name) return true;
		current = current->next;
	}
	return false;
}
bool database::Is_brand_there(string name, string title) {
	if (!Is_company_there(name)) return false;
	node* current = Get_Cdown(name)->next;
	if (!current) return false;
	while (current) {
		if (current->N == title) return true;
		current = current->next;
	}
	return false;
}
bool database::Is_model_there(string name, string title1, string title2) {
	if (!Is_company_there(name)) return false;
	if (!Is_brand_there(name, title1)) return false;
	node* current = Get_Bdown(Get_Cdown(name), title1)->next;
	if (!current) return false;
	while (current) {
		if (current->N == title2) return true;
		current = current->next;
	}
	return false;
}
bool database::Is_vehicle_there(string name, string title1, string title2, string licence) {
	if (!Is_company_there(name)) return false;
	if (!Is_brand_there(name, title1)) return false;
	if (!Is_model_there(name, title1, title2)) return false;
	node* current = Get_Mdown(Get_Bdown(Get_Cdown(name), title1), title2)->next;
	if (!current) return false;
	while (current) {
		if (current->A == licence) return true;
		current = current->next;
	}
	return false;
}

node* database::Get_Cdown(string name) {
	node* current = root->next;
	while (current) {
		if (current->N == name)
			return current->down;
		current = current->next;
	}
}
node* database::Get_Bdown(node* x, string title) {
	node* current = x->next;
	while (current) {
		if (current->N == title)
			return current->down;
		current = current->next;
	}
}
node* database::Get_Mdown(node* x, string title) {
	node* current = x->next;
	while (current) {
		if (current->N == title)
			return current->down;
		current = current->next;
	}
}

void database::Clear_list() {
	node* x = root;
	if (!(x->next)) { delete x; return; }
	if (x->next->type < 4) {
		node* r = x->next;
		node* r2 = r->next;
		while (r2) {
			Remove_the_hole_node(r->down);
			delete r;
			r = r2;
			r2 = r2->next;
		}
		Remove_the_hole_node(r->down);
		delete r;
	}
	else {
		node* r = x->next;
		node* r2 = r->next;
		while (r2) {
			delete r;
			r = r2;
			r2 = r2->next;
		}
		delete r;
	}
	delete x;
	root = new node();
}

string database::Check_a_vehicle(string licence) {
	node* c = root->next;
	string cname = c->N;
	while (c) {
		node* b = c->down->next;
		string bname = b->N;
		while (b) {
			node* m = b->down->next;
			string mname = m->N;
			while (m) {
				node* v = m->down->next;
				string vname = ("(" + cname + ", " + bname + ", " + mname + ", " 
					+ v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
				while (v) {
					if (v->A == licence)
						return vname;
					v = v->next;
					if (v) string vname = ("("+cname+", " + bname + ", " +
						mname + ", " + v->A + "," + v->N + ", " + to_string(v->Y) 
						+ "," + to_string(v->M) + ")");
				}
				m = m->next;
				string mname = m->N;
			}
			b = b->next;
			string bname = b->N;
		}
		c = c->next;
		string cname = c->N;
	}
	return "such vehicle doesn't exist";
}

void database::print_a_m_of_database(string name, string title1, string title2) {
	if (!Is_model_there(name, title1, title2)) { cout << "the model doesn't exist!" << endl; return; }
	cout << "			Model: " << title2 << "= {" << endl;
	node* m = Get_Mdown(Get_Bdown(Get_Cdown(name), title1), title2);
	node* v = m->next;
	if (v) {
		string vname = ("(" + v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
		while (v) {
			cout << "				" << vname << endl;
			v = v->next;
			if (v) string vname = ("(" + v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
		}
	}
	cout << "			" << "}" << endl;
}
void database::print_a_b_of_database(string name, string title) {
	if (!Is_brand_there(name, title)) { cout << "the brand doesn't exist!" << endl; return; }
	cout << "		Brand: " << title << "= {" << endl;
	node* b = Get_Bdown(Get_Cdown(name), title);
	node* m = b->next;
	while (m) {
		print_a_m_of_database(name, title, m->N);
		m = m->next;
	}
	cout << "		" << "}" << endl;
}
void database::print_a_c_of_database(string name) {
	if (!Is_company_there(name)) { cout << "the company doesn't exist!" << endl; return; }
	cout << "	Company: " << name << "= {" << endl;
	node* c = Get_Cdown(name);
	node* b = c->next;
	while (b) {
		print_a_b_of_database(name, b->N);
		b = b->next;
	}
	cout << "	" << "}" << endl;
}
void database::print_database() {
	cout << "database = {" << endl;
	node* r = root->next;
	while (r) {
		print_a_c_of_database(r->N);
		r = r->next;
	}
	cout << "}" << endl;
}

void database::Vihecles_with_specified_color(string color) {
	cout << "color->" << color << "= {" << endl;
	node* c = root->next;
	string cname = c->N;
	while (c) {
		node* b = c->down->next;
		string bname = b->N;
		while (b) {
			node* m = b->down->next;
			string mname = m->N;
			while (m) {
				node* v = m->down->next;
				string vname = ("(" + cname + ", " + bname + ", " + mname + ", "
					+ v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
				while (v) {
					if (v->N == color)
						cout << vname << endl;
					v = v->next;
					string vname = ("(" + cname + ", " + bname + ", " + mname + ", "
						+ v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
				}
				m = m->next;
				string mname = m->N;
			}
			b = b->next;
			string bname = b->N;
		}
		c = c->next;
		string cname = c->N;
	}
	cout << "}" << endl;
}
void database::Vihecles_with_specified_year_produced(int year) {
	cout << "year->" << year << "= {" << endl;
	node* c = root->next;
	string cname = c->N;
	while (c) {
		node* b = c->down->next;
		string bname = b->N;
		while (b) {
			node* m = b->down->next;
			string mname = m->N;
			while (m) {
				node* v = m->down->next;
				string vname = ("(" + cname + ", " + bname + ", " + mname + ", "
					+ v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
				while (v) {
					if (v->Y == year)
						cout << vname << endl;
					v = v->next;
					if (v) string vname = ("(" + cname + ", " + bname + ", " + mname + ", "
						+ v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
				}
				m = m->next;
				string mname = m->N;
			}
			b = b->next;
			string bname = b->N;
		}
		c = c->next;
		string cname = c->N;
	}
	cout << "}" << endl;

}
void database::Vihecles_with_specified_mileage(int low, int high) {
	cout << "mileage ->" << low << " to " << high << "= {" << endl;
	node* c = root->next;
	string cname = c->N;
	while (c) {
		node* b = c->down->next;
		string bname = b->N;
		while (b) {
			node* m = b->down->next;
			string mname = m->N;
			while (m) {
				node* v = m->down->next;
				string vname = ("(" + cname + ", " + bname + ", " + mname + ", "
					+ v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
				while (v) {
					if ((low <= v->M) && ((v->M) <= high))
						cout << vname << endl;
					v = v->next;
					string vname = ("(" + cname + ", " + bname + ", " + mname + ", "
						+ v->A + ", " + v->N + ", " + to_string(v->Y) + ", " + to_string(v->M) + ")");
				}
				m = m->next;
				string mname = m->N;
			}
			b = b->next;
			string bname = b->N;
		}
		c = c->next;
		string cname = c->N;
	}
	cout << "}" << endl;

}

//functions

string Tolower(string s) {
	string x = s;
	for (size_t i = 0; i < x.length(); i++)
	{
		if (int(x[i]) > 64 && x[i] < 91)
			x[i] = char(int(x[i]) + 32);
	}
	return x;
}
string strip(string s) {
	int j = (s.length()) - 3;
	char* ptr = new char[j];
	for (int i = 0; i < j; i++) {
		ptr[i] = s.at(i + 2);
	}
	ptr[j] = '\0';
	if (ptr[j - 1] == '\"') { ptr[j - 1] = '\0'; }
	string h = ptr;
	return h;
}
string Get_the_data(string x) {
	istringstream s(x);
	string k;
	getline(s, k, ':');
	getline(s, k, ':');
	k = strip(k);
	return k;
}
bool Check_licence(string x) {
	if (x.length() != 9)
		return false;
	if ((x.at(0) > 57) && (x.at(0) < 48))
		return false;
	if ((x.at(1) > 57) && (x.at(1) < 48))
		return false;
	if ((x.at(2) > 57) && (x.at(2) < 48))
		return false;
	if ((x.at(4) > 57) && (x.at(4) < 48))
		return false;
	if ((x.at(5) > 57) && (x.at(5) < 48))
		return false;
	if (x.at(6)!='/')
		return false;
	if ((x.at(7) > 57) && (x.at(7) < 48))
		return false;
	if ((x.at(8) > 57) && (x.at(8) < 48))
		return false;
	return true;
}

int menu() {
	cout << "Hello and wlecome to my program! How can I help you?" << endl;
	cout << "0.exit" << endl
		<< "1.Adding a new company or brand or car to database" << endl
		<< "2.Removing a company or brand or car from database" << endl
		<< "3.Clearing all the database" << endl
		<< "4.Check if a car is in the database" << endl
		<< "5.Print a part of database" << endl
		<< "6.Check the cars with a specified property" << endl
		<< "7.print the databse" << endl
		<< "8.Save database as a json file" << endl;
	string a;
	while (true) {
		cout << ">>";
		cin >> a;
		if ((a != "0") && (a != "1") && (a != "2") && (a != "3") && (a != "4") && (a != "5")
			&& (a != "6") && (a != "7") && (a != "8")) {
			cerr << "You must type a number between 0 to 8!" << endl;
			continue;
		}
		else
			break;
	}
	return stoi(a);
}
void Insert_C(database& D) {
	string f, g;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << endl << "please insert the address of the company>>";
		cin.ignore();
		getline(cin, g);
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	D.Insert_company(f, g);
}
void Insert_B(database& D) {
	string f, g;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << endl << "Please insert the title of the brand>>";
		cin >> g;
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	D.Insert_brand(f, g);
}
void Insert_M(database& D) {
	string f, g, h;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << endl << "Please insert the title of the brand>>";
		cin >> g;
		cout << endl << "Please insert the title of the model>>";
		cin >> h;
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	h = Tolower(h);
	D.Insert_model(f, g, h);
}
void Insert_V(database& D) {
	string f, g, h, c, l;
	int y, m;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << endl << "Please insert the title of the brand>>";
		cin >> g;
		cout << endl << "Please insert the title of the model>>";
		cin >> h;
		cout << endl << "Please insert the color of the vehicle>>";
		cin >> c;
		cout << endl << "Please insert the licence of the vehicle>>";
		while (true) {
			cin >> l;
			if (Check_licence(l))
				break;
			else
				cout << "The licence you entered is not valid!" << endl;
		}
		cout << endl << "Please insert the year of the vehicle was produced>>";
		while (true) {
			cin >> y;
			if (y > 1146)
				break;
			else
				cout << "The year number you entered is weird "
				<<"because the first vehicle in the world was invented after the 1146" << endl;
		}
		cout << endl << "Please insert the mileage of the vehiclel>>";
		cin >> m;
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	h = Tolower(h);
	l = Tolower(l);
	c = Tolower(c);
	D.Insert_vehicle(f, g, h, y, c, l, m);
}

void Remove_C(database& D) {
	string f;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	D.Remove_company(f);
}
void Remove_B(database& D) {
	string f, g;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << endl << "Please insert the title of the brand>>";
		cin >> g;
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	D.Remove_brand(f, g);
}
void Remove_M(database& D) {
	string f, g, h;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << endl << "Please insert the title of the brand>>";
		cin >> g;
		cout << endl << "Please insert the title of the model>>";
		cin >> h;
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	h = Tolower(h);
	D.Remove_model(f, g, h);
}
void Remove_V(database& D) {
	string f, g, h, l;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin >> f;
		cout << endl << "Please insert the title of the brand>>";
		cin >> g;
		cout << endl << "Please insert the title of the model>>";
		cin >> h;
		cout << endl << "Please insert the licence of the vehicle>>";
		cin >> l;
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	h = Tolower(h);
	l = Tolower(l);
	D.Remove_vehicle(f, g, h, l);
}

void F1_reading_json_file(database& D) {
	ifstream f;
	f.open("D:/data.txt");
	string s;
	string name, title1, title2;
	while (getline(f, s)) {
		if (s.find("Name") < s.length()) {
			string o1 = Tolower(Get_the_data(s));
			getline(f, s);
			string o2 = Tolower(Get_the_data(s));
			D.Insert_company(o1, o2);
			name = o1;
		}
		else if (s.find("Model") < 25) {
			string o1 = Tolower(Get_the_data(s));
			D.Insert_brand(name, o1);
			title1 = o1;
		}
		else if (s.find("Model") < s.length()) {
			string o1 = Tolower(Get_the_data(s));
			D.Insert_model(name, title1, o1);
			title2 = o1;
		}
		else if (s.find("year") < s.length()) {
			int year = stoi(Get_the_data(s));
			getline(f, s);
			string color = Tolower(Get_the_data(s));
			getline(f, s);
			int mileage = stoi(Get_the_data(s));
			getline(f, s);
			string licence = Get_the_data(s);
			D.Insert_vehicle(name, title1, title2, year, color, licence, mileage);
		}
	}
	f.close();
}
void F2_Adding_a_new_data(database& D) {
	cout << "What kind of data you want to add to database?" << endl
		<< "0.return" << endl
		<< "1.A company" << endl << "2.A brand" << endl
		<< "3.A model" << endl << "4.A vehicle" << endl
		<< "Important note:if you want to add a brand you should add its company before it and it"
		<< " goes the same with model and vehicle" << endl;
	string a;
	while (true) {
		cout << ">>";
		cin >> a;
		if ((a != "0") && (a != "1") && (a != "2") && (a != "3") && (a != "4")) {
			cerr << "You must type a number between 0 to 4!" << endl;
			continue;
		}
		else
			break;
	}
	int i = stoi(a);
	if (!i)	return;
	if (i == 1) Insert_C(D);
	if (i == 2) Insert_B(D);
	if (i == 3) Insert_M(D);
	if (i == 4) Insert_V(D);
}
void F3_Removing_a_data(database& D) {
	cout << "What kind of data you want to remove from database?" << endl
		<< "0.return" << endl
		<< "1.A company" << endl << "2.A brand" << endl
		<< "3.A model" << endl << "4.A vehicle" << endl
		<< "Important note:if you remove a data that doesn't exist yet in the database it doesn't "
		<< "show you any error message" << endl;
	string a;
	while (true) {
		cout << ">>";
		cin >> a;
		if ((a != "0") && (a != "1") && (a != "2") && (a != "3") && (a != "4")) {
			cerr << "You must type a number between 0 to 4!" << endl;
			continue;
		}
		else
			break;
	}
	int i = stoi(a);
	switch (i)
	{
	case 0:
		return;
	case 1:
		Remove_C(D);
		break;
	case 2:
		Remove_B(D);
		break;
	case 3:
		Remove_M(D);
		break;
	case 4:
		Remove_V(D);
		break;
	}
}
void F4_Clear_all_database(database& D) {
	D.Clear_list();
}
void F5_Check_a_car(database& D) {
	string licence;
	cout << "Please enter licence of the vehicle you're looking for:";
	cin >> licence;
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl
		<< D.Check_a_vehicle(licence) << endl
		<< "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl << endl;
}
void printing_c_part(database& D) {
	string f;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin.ignore();
		getline(cin, f);
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	D.print_a_c_of_database(f);
}
void printing_b_part(database& D) {
	string f, g;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin.ignore();
		getline(cin, f);
		cout << endl << "Please insert the title of the brand>>";
		//cin.ignore();
		getline(cin, g);
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	D.print_a_b_of_database(f, g);
}
void printing_m_part(database& D) {
	string f, g, h;
	while (true) {
		cout << "Please insert the name of the company>>";
		cin.ignore();
		getline(cin, f);
		cout << endl << "Please insert the title of the brand>>";
		cin.ignore();
		getline(cin, g);
		cout << endl << "Please insert the title of the model>>";
		cin.ignore();
		getline(cin, h);
		cout << "Do you confirm the data you entered is correct?" << endl
			<< "1.Yes" << endl << "2.No" << endl;
		string a;
		while (true) {
			cout << ">>";
			cin >> a;
			if ((a != "1") && (a != "2")) {
				cerr << "You must type 1 or 2!" << endl;
				continue;
			}
			else
				break;
		}
		if (a == "1")break;
	}
	f = Tolower(f);
	g = Tolower(g);
	h = Tolower(h);
	D.print_a_m_of_database(f, g, h);

}
void F6_Print_a_part_of_database(database& D) {
	cout << "which kind of data is the part you want to print?" << endl
		<< "0.return" << endl
		<< "1.A company" << endl << "2.A brand" << endl
		<< "3.A model" << endl;
	string a;
	while (true) {
		cout << ">>";
		cin >> a;
		if ((a != "0") && (a != "1") && (a != "2") && (a != "3")) {
			cerr << "You must type a number between 0 to 3!" << endl;
			continue;
		}
		else
			break;
	}
	int i = stoi(a);
	switch (i)
	{
	case 0:
		return;
	case 1:
		printing_c_part(D);
		break;
	case 2:
		printing_b_part(D);
		break;
	case 3:
		printing_m_part(D);
		break;
	}
}
void F7_Vihecles_with_a_specified_year_produced(database& D) {
	int f;
	cout << "Please insert the year>>";
	cin >> f;
	D.Vihecles_with_specified_year_produced(f);
}
void F8_Vihecles_with_a_specified_color(database& D) {
	string f;
	cout << "Please insert the color>>";
	cin >> f;
	D.Vihecles_with_specified_color(f);
}
void F9_Vihecles_with_a_specified_mileage(database& D) {
	int f, g;
	cout << "Please set your range of mileage>>" << endl;
	cout << "low>>";
	cin >> f;
	cout << "high>>";
	cin >> g;
	D.Vihecles_with_specified_mileage(f, g);
}
void F10_Print_the_database(database& D) {
	D.print_database();
}
void F11_Save_the_data_on_a_json_file(database& D) {
	ofstream f;
	f.open("D:/DS_project1_edited.txt");
	f << '{' << endl;
	node* c1 = D.Get_root()->next;
	if (c1) {
		f << "    " << "\"Company\": [" << endl;
	}
	while (c1) {
		f << "        " << "{" << endl;
		f << "            " << "\"Name\": \"" << c1->N << "\"," << endl;
		f << "            " << "\"Address\": \"" << c1->A << "\"," << endl;
		node* c2 = c1->get_down()->next;
		if (c2) {
			f << "            " << "\"Brand\": [" << endl;
		}
		while (c2) {
			f << "                " << "{" << endl;
			f << "                    " << "\"Model\": \"" << c2->N << "\"," << endl;
			node* c3 = c2->get_down()->next;
			if (c3) {
				f << "                    " << "\"Brand\": [" << endl;
			}
			while (c3) {
				f << "                        " << "{" << endl;
				f << "                            " << "\"Model\": \"" << c3->N << "\"," << endl;
				node* c4 = c3->get_down()->next;
				if (c4) {
					f << "                        " << "\"Vehicle\": [" << endl;
				}
				while (c4) {
					f << "                            " << "{" << endl;
					f << "                                " << "\"year\": \"" << c4->Y << "\"," << endl;
					f << "                                " << "\"Color\": \"" << c4->N << "\"," << endl;
					f << "                                " << "\"Mileage\": \"" << c4->M << "\"," << endl;
					f << "                                " << "\"Licence\": \"" << c4->A << "\"," << endl;
					f << "                            " << "}," << endl;
					c4 = c4->next;
				}
				if (c4) {
					f << "                        " << "]" << endl;
				}
				c3 = c3->next;
			}
			if (c3) {
				f << "                    " << "]" << endl;
			}
			f << "        " << "}," << endl;
			c2 = c2->next;
		}
		if (c2) {
			f << "            " << "]" << endl;
		}
		c1 = c1->next;
	}
	if (c1) {
		f << "    " << "]" << endl;
	}
	f << '}';
	f.close();
}


void P_menu(database& D) {
	cout << "waht kind of filter you want?" << endl;
	cout << "1.color" << endl
		<< "2.year produced" << endl
		<< "3.mileage" << endl;
	int f;
	cin >> f;
	switch (f)
	{
	case 1:
		F8_Vihecles_with_a_specified_color(D);
		break;
	case 2:
		F7_Vihecles_with_a_specified_year_produced(D);
		break;
	case 3:
		F9_Vihecles_with_a_specified_mileage(D);
		break;
	}
}

//Main
int main() {
	database D;
	F1_reading_json_file(D);
	while (true) {
		int i = menu();
		if (i == 0) exit(0);
		else if (i == 1)
			F2_Adding_a_new_data(D);
		else if (i == 2)
			F3_Removing_a_data(D);
		else if (i == 3)
			F4_Clear_all_database(D);
		else if (i == 4)
			F5_Check_a_car(D);
		else if (i == 5)
			F6_Print_a_part_of_database(D);
		else if (i == 6)
			P_menu(D);
		else if (i == 7)
			F10_Print_the_database(D);
		else if (i == 8)
			F11_Save_the_data_on_a_json_file(D);
		cin.ignore();
	}
	return 0;
}