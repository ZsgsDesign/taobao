#include "stdafx.h"

using namespace std;

MySQLInterface database_conn;

struct user {
	int uid;
	string name;
	string email;
	string password;
	bool admin;
}userinfo;

void actionViewProduct(string);
void actionViewCatgById(string);
void actionAddCart(string);
void actionCheckout();
void actionClearCart();
void actionViewCartItem(string);
void actionModifyCartItem(string);
void actionRemoveCartItem(string);
void actionViewOrderItem(string);
void actionPayOrder(string);
void actionCancelOrder(string);
void actionViewBalance();
void actionManageItems();
void actionAddItem();
void actionModifyItem(string);
void actionDeleteItem(string);

void str2int(int &int_temp, const string &string_temp)	// "1"=>1
{
	stringstream stream(string_temp);
	stream >> int_temp;
}

void int2str(const int &int_temp, string &string_temp)
{
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();
}

int str2int_sync(const string &string_temp)
{
	int int_temp;
	stringstream stream(string_temp);
	stream >> int_temp;
	return int_temp;
}

string int2str_sync(const int &int_temp)
{
	stringstream stream;
	stream << int_temp;
	return stream.str();
}

string str2money_sync(string string_temp) {
	if (string_temp.substr(0, 1) == "-") {
		if (string_temp.size() == 1) {
			return "-";
		}
		else if (string_temp.size() == 2) {
			return "-0.0" + string_temp.substr(1, string_temp.size() - 1);
		}
		else if (string_temp.size() == 3) {
			return "-0." + string_temp.substr(1, string_temp.size() - 1);
		}
		else {
			return string_temp.substr(0, string_temp.size() - 2) + "." + string_temp.substr(string_temp.size() - 2, 2);
		}
	}
	else {
		if (string_temp.size() == 0) {
			return "";
		}
		else if (string_temp.size() == 1) {
			return "0.0" + string_temp;
		}
		else if (string_temp.size() == 2) {
			return "0." + string_temp;
		}
		else {
			return string_temp.substr(0, string_temp.size() - 2) + "." + string_temp.substr(string_temp.size() - 2, 2);
		}
	}
}


void actiondisplay(string tips = "0") {
	system("cls");
	cout << endl << "\t欢迎来到 淘不了宝 综合电商平台" << endl << endl;
	cout << "\tKernel Version" << endl << endl;
	if (tips != "0") cout << "\t> " << tips << endl << endl;
}

bool actionLogin() {
	actiondisplay();
	cout << "\t请输入邮箱：";
	//cin.ignore();
	string temp_user_email;
	getline(cin,temp_user_email);
	cout << "\t请输入密码：";
	string temp_pass;
	char temp_pass_digit;
	while (temp_pass_digit = _getch()) {
		if (temp_pass_digit == '\n' || temp_pass_digit == '\r') {
			break;
		}
		else if (temp_pass_digit == '\b') {
			if (temp_pass.size()) {
				temp_pass.erase(temp_pass.end() - 1);
				actiondisplay();
				cout << "\t请输入邮箱：" << temp_user_email << endl << "\t请输入密码：";
				for (unsigned int i = 0; i < temp_pass.size(); i++) {
					cout << "*";
				}
			}
		}
		else {
			temp_pass += temp_pass_digit;
			actiondisplay();
			cout << "\t请输入邮箱：" << temp_user_email << endl << "\t请输入密码：";
			for (unsigned int i = 0; i < temp_pass.size(); i++) {
				cout << "*";
			}
		}
	}
	MD5 temp_pass_md5 = MD5(temp_pass);
	string temp_pass_md5_value = temp_pass_md5.hexdigest();
	vector<vector<string> > data;
	string sqlstr = "SELECT uid,name,email,password,admin from users where email='" + temp_user_email + "' and password='" + temp_pass_md5_value + "'";
	database_conn.Select(sqlstr, data);

	if (data.size()) {
		str2int(userinfo.uid, data[0][0]);
		userinfo.name = data[0][1];
		userinfo.email = data[0][2];
		userinfo.password = data[0][3];
		userinfo.admin = str2int_sync(data[0][4]);
		ofstream fout("user.config");
		fout << userinfo.email << "\t" << userinfo.password << endl;
		fout.close();
		return true;
	}
	else {
		return false;
	}
}

int actionRegister() {
	actiondisplay();
	cout << "\t请输入您的邮箱：";
	string temp_user_email;
	getline(cin, temp_user_email);
	regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
	if (!regex_match(temp_user_email, pattern)) {
		return -1;
	}
	cout << "\t请输入您的用户名：";
	string temp_user_name;
	getline(cin, temp_user_name);
	if (temp_user_name == "") {
		return -2;
	}
	cout << "\t请设置一个密码：";
	string temp_pass;
	char temp_pass_digit;
	while (temp_pass_digit = _getch()) {
		if (temp_pass_digit == '\n' || temp_pass_digit == '\r') {
			break;
		}
		else if (temp_pass_digit == '\b') {
			if (temp_pass.size()) {
				temp_pass.erase(temp_pass.end() - 1);
				actiondisplay();
				cout << "\t请输入您的邮箱：" << temp_user_email << endl << "\t请输入您的用户名：" << temp_user_name << endl << "\t请设置一个密码：";
				for (unsigned int i = 0; i < temp_pass.size(); i++) {
					cout << "*";
				}
			}
		}
		else {
			temp_pass += temp_pass_digit;
			actiondisplay();
			cout << "\t请输入您的邮箱：" << temp_user_email << endl << "\t请输入您的用户名：" << temp_user_name << endl << "\t请设置一个密码：";
			for (unsigned int i = 0; i < temp_pass.size(); i++) {
				cout << "*";
			}
		}
	}
	if (temp_pass.size() < 6) {
		return -3;
	}
	MD5 temp_pass_md5 = MD5(temp_pass);
	string temp_pass_md5_value = temp_pass_md5.hexdigest();
	vector<vector<string> > data;
	string sqlstr = "SELECT * from users where email='" + temp_user_email + "'";
	database_conn.Select(sqlstr, data);

	if (data.size()) {
		return 0;
	}
	else {
		string sqlstr = "insert into users set email='" + temp_user_email + "', name='" + temp_user_name + "',password='" + temp_pass_md5_value + "',admin=0";
		database_conn.Query(sqlstr);
		ofstream fout("user.config");
		fout << temp_user_email << "\t" << temp_pass_md5_value << endl;
		fout.close();
		return 1;
	}
}

string actionShowMenu(string level) {
	actiondisplay("欢迎回来，" + userinfo.name + "！");
	if (level == "0") {

		// 销量排名前五的商品随机选择一个展示为推荐

		vector<vector<string> > data;
		string sqlstr = "(SELECT p.pid,p.name from order_details o left join product p on o.pid=p.pid where p.available=1 GROUP BY pid ORDER BY SUM(count) DESC limit 5) order by rand() limit 1;"; 
		database_conn.Select(sqlstr, data);

		// 格式化输出

		int max_len = max(8, data[0][1].length() + data[0][0].length() + 4);
		max_len += 4;
		cout << "\t+";
		for (int i = 2; i <= max_len-1; i++) cout << "-";
		cout << "+" << endl;
		cout << "\t| 特别推荐";
		for (int i = 11; i <= max_len-2; i++) cout << " ";
		cout << " |" << endl;
		cout << "\t| [0" + data[0][0] + "] " << data[0][1];
		for (int i = data[0][1].length() + data[0][0].length() + 7; i <= max_len - 2; i++) cout << " ";
		cout << " |" << endl;
		cout << "\t+";
		for (int i = 2; i <= max_len-1; i++) cout << "-";
		cout << "+" << endl << endl;

		// 普通菜单项

		cout << "\t[1] 关键词查找宝贝" << endl;
		cout << "\t[2] 浏览宝贝分类" << endl;
		cout << "\t--------" << endl;
		cout << "\t[3] 关键词查找用户" << endl;
		cout << "\t--------" << endl;
		cout << "\t[4] 我的订单" << endl;
		cout << "\t[5] 购物车" << endl;
		cout << "\t[6] 我的钱包" << endl;
		cout << "\t[7] 注销并退出" << endl;
		if (userinfo.admin) {
			cout << "\t--------" << endl;
			cout << "\t[8] 管理商品" << endl;
		}
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
		return data[0][0];
	}
	else if (level == "01") {
		cout << "\t[0] 返回上级" << endl << endl;
		cout << endl << "\t----------------" << endl << "\t请输入一个关键词：";
	}
	return "default";
}

void actionSearchUserByKeyWord(string keyword) {
	if (keyword == "") {
		actiondisplay("关键词不能为空");
		cout << "\t";
		system("pause");
		return;
	}

	vector<vector<string> > data;
	string sqlstr = "SELECT uid,name,email from users where name like '%" + keyword + "%' ";
	database_conn.Select(sqlstr, data);

	if (data.size() > 0) {
		actiondisplay("为您找到如下结果");
		cout << "\t序号\t用户名\t\t用户邮箱" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				cout << data[i][j];
				cout << "\t";
			}
			cout << endl;
		}
	}
	else {
		actiondisplay("关键词没有找到任何用户");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		actiondisplay("为您找到如下结果");
		cout << "\t序号\t用户名\t\t用户邮箱" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				cout << data[i][j];
				cout << "\t";
			}
			cout << endl;
		}
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

void actionSearchByKeyWord(string keyword) {
	if (keyword == "") {
		actiondisplay("关键词不能为空");
		cout << "\t";
		system("pause");
		return;
	}

	vector<vector<string> > data;
	string sqlstr = "SELECT pid,c.name as catg_name,p.price,p.brand,p.name from product p left join category c on c.cid=p.catg_id where p.available=1 and p.name like '%" + keyword + "%' ";
	database_conn.Select(sqlstr, data);

	if (data.size() > 0) {
		actiondisplay("为您找到如下结果");
		cout << "\t序号\t分类\t价格\t品牌\t产品名" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				if (j == 0)cout << "[";
				if (j != 2) cout << data[i][j];
				else cout << str2money_sync(data[i][j]);
				if (j == 0)cout << "]";
				cout << "\t";
			}
			cout << endl;
		}
	}
	else {
		actiondisplay("关键词没有找到任何结果");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		bool find_match = false;
		for (unsigned int i = 0; i < data.size(); ++i) {
			if (data[i][0] == temp_string) {
				find_match = true;
				break;
			}
		}
		if (find_match) {
			actionViewProduct(temp_string);
		}
		actiondisplay("为您找到如下结果");
		cout << "\t序号\t分类\t价格\t品牌\t产品名" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				if (j == 0)cout << "[";
				if (j != 2) cout << data[i][j];
				else cout << str2money_sync(data[i][j]);
				if (j == 0)cout << "]";
				cout << "\t";
			}
			cout << endl;
		}
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

void actionSearch() {
	string keyword;
	actionShowMenu("01");
	while (getline(cin, keyword)) {
		if (keyword == "0") {
			return;
		}
		else {
			actionSearchByKeyWord(keyword);
			actionShowMenu("01");
		}
	}
}

void actionSearchUser() {
	string keyword;
	actionShowMenu("01");
	while (getline(cin, keyword)) {
		if (keyword == "0") {
			return;
		}
		else {
			actionSearchUserByKeyWord(keyword);
			actionShowMenu("01");
		}
	}
}

void actionViewCatg() {
	actiondisplay("全部分类");
	vector<vector<string> > data;
	string sqlstr = "SELECT cid,name from category";
	database_conn.Select(sqlstr, data);
	for (unsigned int i = 0; i < data.size(); ++i) {
		cout << "\t";
		for (unsigned int j = 0; j < data[0].size(); ++j) {
			if (j == 0)cout << "[";
			cout << data[i][j];
			if (j == 0)cout << "] ";
		}
		cout << endl;
	}
	cout << "\t--------" << endl;
	cout << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		bool find_match = false;
		for (unsigned int i = 0; i < data.size(); ++i) {
			if (data[i][0] == temp_string) {
				find_match = true;
				break;
			}
		}
		if (find_match) {
			actionViewCatgById(temp_string);
		}
		database_conn.Select(sqlstr, data);
		actiondisplay("全部分类");
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				if (j == 0)cout << "[";
				cout << data[i][j];
				if (j == 0)cout << "] ";
			}
			cout << endl;
		}
		cout << "\t--------" << endl;
		cout << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

void actionViewOrder() {
	vector<vector<string> > data;
	string sqlstr = "SELECT o.oid,o.`status`,SUM(count) tot_quantity,SUM(unit_price*count) tot_price,p.`name` from `order` o left join order_details d on o.oid=d.oid left join product p on d.pid=p.pid left join category c on c.cid=p.catg_id where uid="+ int2str_sync(userinfo.uid) +" group by o.oid order by create_date DESC";
	database_conn.Select(sqlstr, data);

	if (data.size() > 0) {
		actiondisplay("我的订单");
		cout << "\t序号\t订单状态\t总价格\t\t购买商品" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {

			cout << "\t";

			cout << "[" << data[i][0] << "]" << "\t";
			if (data[i][1] == "1") {
				cout << "已支付" << "\t";
			}
			else if (data[i][1] == "0") {
				cout << "支付中" << "\t";
			}
			else if (data[i][1] == "-1") {
				cout << "已取消" << "\t";
			}
			cout << "\t";
			cout << "￥" << str2money_sync(data[i][3]) << ((str2money_sync(data[i][3]).length() >= 6) ? "\t" : "\t\t");
			cout  << data[i][4];
			if (str2int_sync(data[i][2]) > 1) {
				cout << " 等" << data[i][2] << "件商品";
			}

			cout << endl;
		}
	}
	else {
		actiondisplay("您还没有任何订单哦");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		bool find_match = false;
		for (unsigned int i = 0; i < data.size(); ++i) {
			if (data[i][0] == temp_string) {
				find_match = true;
				break;
			}
		}
		if (find_match) {
			actionViewOrderItem(temp_string);
		}

		database_conn.Select(sqlstr, data);
		if (data.size() > 0) {
			actiondisplay("我的订单");
			cout << "\t序号\t订单状态\t总价格\t\t购买商品" << endl << endl;
			for (unsigned int i = 0; i < data.size(); ++i) {

				cout << "\t";

				cout << "[" << data[i][0] << "]" << "\t";
				if (data[i][1] == "1") {
					cout << "已支付" << "\t";
				}
				else if (data[i][1] == "0") {
					cout << "支付中" << "\t";
				}
				else if (data[i][1] == "-1") {
					cout << "已取消" << "\t";
				}
				cout << "\t";
				cout << "￥" << str2money_sync(data[i][3]) << ((str2money_sync(data[i][3]).length() >= 6) ? "\t" : "\t\t");
				cout << data[i][4];
				if (str2int_sync(data[i][2]) > 1) {
					cout << " 等" << data[i][2] << "件商品";
				}

				cout << endl;
			}
		}
		else {
			actiondisplay("您还没有任何订单哦");
			cout << "\t";
			system("pause");
			return;
		}
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";
	}
}

void actionInpour() {
	actiondisplay("充值");

	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请输入充值金额：￥";
	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") return;
		regex pattern("(\\d{1,9})");
		if (regex_match(temp_string, pattern)) {
			string sqlstr = "insert into transaction set uid = " + int2str_sync(userinfo.uid) + ", diff = 100*" + temp_string + ", comment = '充值',trans_time = now()"; 
			database_conn.Query(sqlstr);
			actiondisplay("充值成功！");
			cout << "\t";
			system("pause");
			return;
		}
		actiondisplay("充值金额不合法！");
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请输入一个合法的数量：";
	}
}

void actionViewBalance() {
	map<string,vector<vector<string> > > data;
	string transaction_general = "SELECT SUM(diff) from transaction where uid =" + int2str_sync(userinfo.uid);
	database_conn.Select(transaction_general, data["general"]);
	string transaction_detail = "SELECT tid,diff,trans_time,`comment` from transaction where uid =" + int2str_sync(userinfo.uid);
	database_conn.Select(transaction_detail, data["detail"]);

	if (data.size() > 0) {
		actiondisplay("我的钱包");

		cout << "\t余额：￥" << str2money_sync(data["general"][0][0]) << endl << endl;

		cout << "\t序号\t操作\t时间\t\t\t备注" << endl << endl;
		for (unsigned int i = 0; i < data["detail"].size(); ++i) {
			cout << "\t" << data["detail"][i][0];
			cout << "\t" << ((str2int_sync(data["detail"][i][1]) > 0) ? "+" : "") << str2money_sync(data["detail"][i][1]);
			cout << "\t" << data["detail"][i][2];
			cout << "\t" << data["detail"][i][3];
			cout << endl;
		}
	}
	else {
		actiondisplay("我的钱包");
		cout << "\t余额：￥0" << endl << endl;
	}
	cout << endl << "\t[1] 充值" << endl;
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个项目进行操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			actionInpour();
			return;
		}
		database_conn.Select(transaction_general, data["general"]);
		database_conn.Select(transaction_detail, data["detail"]);

		if (data.size() > 0) {
			actiondisplay("我的钱包");

			cout << "\t余额：￥" << str2money_sync(data["general"][0][0]) << endl << endl;

			cout << "\t序号\t操作\t时间\t\t\t备注" << endl << endl;
			for (unsigned int i = 0; i < data["detail"].size(); ++i) {
				cout << "\t" << data["detail"][i][0];
				cout << "\t" << ((str2int_sync(data["detail"][i][1]) > 0) ? "+" : "") << str2money_sync(data["detail"][i][1]);
				cout << "\t" << data["detail"][i][2];
				cout << "\t" << data["detail"][i][3];
				cout << endl;
			}
		}
		else {
			actiondisplay("我的钱包");
			cout << "\t余额：￥0" << endl << endl;
		}
		cout << endl << "\t[1] 充值" << endl;
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个项目进行操作：";
	}
}

void actionViewCart() {
	vector<vector<string> > data;
	string sqlstr = "SELECT caid,quantity,p.price,p.name from cart c left join product p on c.pid=p.pid where uid =" + int2str_sync(userinfo.uid);
	database_conn.Select(sqlstr, data);

	if (data.size() > 0) {
		actiondisplay("购物车");
		cout << "\t序号\t数量\t单价\t\t产品名" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				if (j == 0)cout << "[";
				if (j != 2) cout << data[i][j];
				else cout << "￥" << str2money_sync(data[i][j]) << ((str2money_sync(data[i][j]).length() >= 6) ? "" : "\t");
				if (j == 0)cout << "]";
				cout << "\t";
			}
			cout << endl;
		}
	}
	else {
		actiondisplay("购物车空空如也哦");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[01] 结账" << endl;
	cout << "\t[02] 清空购物车" << endl;
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个购物车项目进行操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "01") {
			actionCheckout();
			return;
		}
		else if (temp_string == "02") {
			actionClearCart();
			return;
		}
		bool find_match = false;
		for (unsigned int i = 0; i < data.size(); ++i) {
			if (data[i][0] == temp_string) {
				find_match = true;
				break;
			}
		}
		if (find_match) {
			actionViewCartItem(temp_string);
		}
		database_conn.Select(sqlstr, data);
		if (data.size() > 0) {
			actiondisplay("购物车");
			cout << "\t序号\t数量\t单价\t\t产品名" << endl << endl;
			for (unsigned int i = 0; i < data.size(); ++i) {
				cout << "\t";
				for (unsigned int j = 0; j < data[0].size(); ++j) {
					if (j == 0)cout << "[";
					if (j != 2) cout << data[i][j];
					else cout << "￥" << str2money_sync(data[i][j]) << ((str2money_sync(data[i][j]).length() >= 6) ? "" : "\t");
					if (j == 0)cout << "]";
					cout << "\t";
				}
				cout << endl;
			}
		}
		else {
			actiondisplay("购物车空空如也哦");
			cout << "\t";
			system("pause");
			return;
		}
		cout << endl << "\t[01] 结账" << endl;
		cout << "\t[02] 清空购物车" << endl;
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个购物车项目进行操作：";
	}
}

void actionViewProduct(string pid) {
	vector<vector<string> > data;
	string sqlstr = "SELECT pid,c.name as catg_name,p.brand,p.name,p.price,p.description,p.detail,p.extra,p.available from product p left join category c on c.cid=p.catg_id where pid=" + pid + " ";
	database_conn.Select(sqlstr, data);
	if (data.size() > 0) {
		if (data[0][8] == "1") {

			actiondisplay("商品详情");

			cout << "\t商品名：" << data[0][3] << endl << endl;
			cout << "\t商品分类：" << data[0][1] << endl << endl;
			cout << "\t商品品牌：" << data[0][2] << endl << endl;
			cout << "\t商品价格：￥" << str2money_sync(data[0][4]) << endl << endl;
			cout << "\t商品简介：" << data[0][5] << endl << endl;
			cout << "\t商品详情：" << data[0][6] << endl << endl;
			cout << "\t其他信息：" << data[0][7] << endl << endl;

		}
		else {
			actiondisplay("商品已下架");
			cout << "\t";
			system("pause");
			return;
		}

	}
	else {
		actiondisplay("商品未找到");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[1] 添加至购物车" << endl;
	if (userinfo.admin) {
		cout << endl << "\t[2] 修改商品" << endl;
		cout << "\t[3] 删除商品" << endl;
	}
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			actionAddCart(pid);
		}
		else if (temp_string == "2") {
			if (userinfo.admin) {
				actionModifyItem(pid);
			}
		}
		else if (temp_string == "3") {
			if (userinfo.admin) {
				actionDeleteItem(pid);
				return;
			}
		}
		database_conn.Select(sqlstr, data);
		actiondisplay("商品详情");

		cout << "\t商品名：" << data[0][3] << endl << endl;
		cout << "\t商品分类：" << data[0][1] << endl << endl;
		cout << "\t商品品牌：" << data[0][2] << endl << endl;
		cout << "\t商品价格：￥" << str2money_sync(data[0][4]) << endl << endl;
		cout << "\t商品简介：" << data[0][5] << endl << endl;
		cout << "\t商品详情：" << data[0][6] << endl << endl;
		cout << "\t其他信息：" << data[0][7] << endl << endl;
		cout << endl << "\t[1] 添加至购物车" << endl;
		if (userinfo.admin) {
			cout << endl << "\t[2] 修改商品" << endl;
			cout << "\t[3] 删除商品" << endl;
		}
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

void actionViewCatgById(string cid) {
	vector<vector<string> > data;
	string sqlstr = "SELECT pid,c.name as catg_name,p.price,p.brand,p.name from product p left join category c on c.cid=p.catg_id where p.available=1 and cid=" + cid + " ";
	database_conn.Select(sqlstr, data);

	if (data.size() > 0) {
		actiondisplay(data[0][1]);
		cout << "\t序号\t价格\t\t品牌\t产品名" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				if (j == 1)continue;
				if (j == 0)cout << "[";
				if (j != 2) cout << data[i][j];
				else cout << "￥" << str2money_sync(data[i][j]) << ((str2money_sync(data[i][j]).length() >= 6) ? "" : "\t");
				if (j == 0)cout << "]";
				cout << "\t";
			}
			cout << endl;
		}
	}
	else {
		actiondisplay("本分类没有找到任何结果");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		bool find_match = false;
		for (unsigned int i = 0; i < data.size(); ++i) {
			if (data[i][0] == temp_string) {
				find_match = true;
				break;
			}
		}
		if (find_match) {
			actionViewProduct(temp_string);
		}
		actiondisplay(data[0][1]);
		cout << "\t序号\t价格\t\t品牌\t产品名" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 0; j < data[0].size(); ++j) {
				if (j == 1) continue;
				if (j == 0) cout << "[";
				if (j != 2) cout << data[i][j];
				else cout << "￥" << str2money_sync(data[i][j]) << ((str2money_sync(data[i][j]).length() >= 6) ? "" : "\t");
				if (j == 0) cout << "]";
				cout << "\t";
			}
			cout << endl;
		}
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

void actionAddCart(string pid) {
	vector<vector<string> > data;
	string sqlstr = "SELECT * from cart where uid=" + int2str_sync(userinfo.uid) + " and pid=" + pid;
	database_conn.Select(sqlstr, data);
	if (data.size() > 0) {
		actiondisplay("购物车中已有本商品！");
		cout << "\t";
		system("pause");
		return;
	}
	else {
		string sqlstr = "insert into cart set uid=" + int2str_sync(userinfo.uid) + ", pid=" + pid + ", quantity=1";
		database_conn.Query(sqlstr);
		actiondisplay("已成功添加至购物车！");
		cout << "\t";
		system("pause");
		return;
	}
}

void actionCheckout() {
	vector<vector<string> > data;
	string sqlstr = "SELECT p.pid,quantity,p.price,p.name from cart c left join product p on c.pid=p.pid where uid =" + int2str_sync(userinfo.uid);
	database_conn.Select(sqlstr, data);

	if (data.size() > 0) {
		actiondisplay("收银台");
		cout << "\t数量\t单价\t\t产品名" << endl << endl;
		for (unsigned int i = 0; i < data.size(); ++i) {
			cout << "\t";
			for (unsigned int j = 1; j < data[0].size(); ++j) {
				if (j != 2) cout << data[i][j];
				else cout << "￥" << str2money_sync(data[i][j]) << ((str2money_sync(data[i][j]).length() >= 6) ? "" : "\t");
				cout << "\t";
			}
			cout << endl;
		}
	}

	else {
		actiondisplay("购物车空空如也哦");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[1] 继续" << endl;
	cout << "\t[0] 取消" << endl;
	cout << endl << "\t----------------" << endl << "\t请确认后继续付款：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			sqlstr = "insert into `order` set uid=" + int2str_sync(userinfo.uid) + ", `status`=0, create_date=now(), ip='127.0.0.1'";
			int oid=database_conn.GetInsertID(sqlstr);
			for (unsigned int i = 0; i < data.size(); ++i) {
				sqlstr = "insert into order_details set oid=" + int2str_sync(oid) + ", pid=" + data[i][0] + ", style=0, count=" + data[i][1] + ", unit_price=" + data[i][2];
				database_conn.Query(sqlstr);
			}
			actiondisplay("已成功创建订单！");
			cout << "\t";
			system("pause");
			actionViewOrderItem(int2str_sync(oid));
			return;
		}

		database_conn.Select(sqlstr, data);

		if (data.size() > 0) {
			actiondisplay("收银台");
			cout << "\t数量\t单价\t\t产品名" << endl << endl;
			for (unsigned int i = 0; i < data.size(); ++i) {
				cout << "\t";
				for (unsigned int j = 1; j < data[0].size(); ++j) {
					if (j != 2) cout << data[i][j];
					else cout << "￥" << str2money_sync(data[i][j]) << ((str2money_sync(data[i][j]).length() >= 6) ? "" : "\t");
					cout << "\t";
				}
				cout << endl;
			}
		}

		else {
			actiondisplay("购物车空空如也哦");
			cout << "\t";
			system("pause");
			return;
		}
		cout << endl << "\t[1] 继续" << endl;
		cout << "\t[0] 取消" << endl;
		cout << endl << "\t----------------" << endl << "\t请确认后继续付款：";
	}
}

void actionViewOrderItem(string oid) {
	map<string,vector<vector<string> >> data;
	string order_detail = "SELECT o.oid,count,unit_price,c.`name` catg_name,p.`name`,p.pid from `order` o left join order_details d on o.oid=d.oid left join product p on d.pid=p.pid left join category c on c.cid=p.catg_id where o.oid=" + oid;
	string order_general = "SELECT o.oid,o.`status`,SUM(count) tot_quantity,SUM(unit_price*count) tot_price,p.`name`,create_date from `order` o left join order_details d on o.oid=d.oid left join product p on d.pid=p.pid left join category c on c.cid=p.catg_id where o.oid=" + oid + " group by o.oid order by create_date DESC";
	database_conn.Select(order_general, data["order_general"]);
	database_conn.Select(order_detail, data["order_detail"]);

	if (data["order_general"].size() > 0) {
		actiondisplay("订单详情");


		cout << "\t订单状态：";
		if (data["order_general"][0][1] == "1") {
			cout << "已支付" << endl;
		}
		else if (data["order_general"][0][1] == "0") {
			cout << "支付中" << endl;
		}
		else if (data["order_general"][0][1] == "-1") {
			cout << "已取消" << endl;
		}
		cout << "\t总价格：" << "￥" << str2money_sync(data["order_general"][0][3]) << endl;
		cout << "\t创建日期：" << data["order_general"][0][5] << endl;

		cout << endl;

		cout << "\t购买商品：" << endl;
		for (unsigned int i = 0; i < data["order_detail"].size(); ++i) {
			cout << endl;
			cout << "\t" << "[" << data["order_detail"][i][3] << "]" << data["order_detail"][i][4] << endl;
			cout << "\t购买件数：" << data["order_detail"][i][1] << endl;
			cout << "\t单价：" << data["order_detail"][i][2] << endl;
		}
		cout << endl;
	}
	else {
		actiondisplay("订单不存在");
		cout << "\t";
		system("pause");
		return;
	}
	if (data["order_general"][0][1] == "0") cout << endl << "\t[1] 继续支付" << endl;
	if (data["order_general"][0][1] == "0") cout << "\t[2] 取消订单" << endl;
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			actionPayOrder(oid);
			return;
		}
		else if (temp_string == "2") {
			actionCancelOrder(oid);
			return;
		}

		database_conn.Select(order_general, data["order_general"]);
		database_conn.Select(order_detail, data["order_detail"]);

		if (data["order_general"].size() > 0) {
			actiondisplay("订单详情");


			cout << "\t订单状态：";
			if (data["order_general"][0][1] == "1") {
				cout << "已支付" << endl;
			}
			else if (data["order_general"][0][1] == "0") {
				cout << "支付中" << endl;
			}
			else if (data["order_general"][0][1] == "-1") {
				cout << "已取消" << endl;
			}
			cout << "\t总价格：" << "￥" << str2money_sync(data["order_general"][0][3]) << endl;
			cout << "\t创建日期：" << data["order_general"][0][5] << endl;

			cout << endl;

			cout << "\t购买商品：" << endl;
			for (unsigned int i = 0; i < data["order_detail"].size(); ++i) {
				cout << endl;
				cout << "\t" << "[" << data["order_detail"][i][3] << "]" << data["order_detail"][i][4] << endl;
				cout << "\t购买件数：" << data["order_detail"][i][1] << endl;
				cout << "\t单价：" << data["order_detail"][i][2] << endl;
			}
			cout << endl;
		}
		else {
			actiondisplay("订单不存在");
			cout << "\t";
			system("pause");
			return;
		}
		if (data["order_general"][0][1] == "0") cout << endl << "\t[1] 继续支付" << endl;
		if (data["order_general"][0][1] == "0") cout << "\t[2] 取消订单" << endl;
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";
	}
}

void actionPayOrder(string oid) {
	map<string,vector<vector<string> > > data;
	string order_general = "SELECT o.oid,SUM(unit_price*count) tot_price from `order` o left join order_details d on o.oid=d.oid left join product p on d.pid=p.pid left join category c on c.cid=p.catg_id where o.oid=" + oid + " group by o.oid order by create_date DESC";
	string transaction = "SELECT SUM(diff) balance from transaction where uid=" + int2str_sync(userinfo.uid);
	database_conn.Select(order_general, data["info"]);
	database_conn.Select(transaction, data["transaction"]);
	actiondisplay("即将支付订单！");
	cout << "\t总价格：" << "￥" << str2money_sync(data["info"][0][1]) << endl;
	cout << "\t您当前的钱包：" << "￥" << str2money_sync(data["transaction"][0][0]) << endl;
	if(str2int_sync(data["transaction"][0][0]) >= str2int_sync(data["info"][0][1])) cout << endl << "\t[1] 支付！" << endl;
	else {
		cout << endl << "\t> 抱歉，账户金额不足！" << endl;
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";
	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			string sqlstr = "update `order` set `status`=1 where oid=" + oid;
			database_conn.Query(sqlstr);
			sqlstr = "insert into transaction set uid=" + int2str_sync(userinfo.uid) + ",diff=-" + data["info"][0][1] + ",comment='支付订单，编号" + oid + "'，trans_time=now()";
			database_conn.Query(sqlstr);
			actiondisplay("支付订单成功！");
			cout << "\t";
			system("pause");
			return;
		}
		database_conn.Select(order_general, data["info"]);
		database_conn.Select(transaction, data["transaction"]);
		actiondisplay("即将支付订单！");
		cout << "\t总价格：" << "￥" << str2money_sync(data["info"][0][1]) << endl;
		cout << "\t您当前的钱包：" << "￥" << str2money_sync(data["transaction"][0][0]) << endl;
		if (str2int_sync(data["transaction"][0][0]) >= str2int_sync(data["info"][0][1])) cout << endl << "\t[1] 支付！" << endl;
		else {
			cout << endl << "\t> 抱歉，账户金额不足！" << endl;
			cout << "\t";
			system("pause");
			return;
		}
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";
	}
}

void actionCancelOrder(string oid) {
	actiondisplay("即将取消订单！");
	cout << endl << "\t[1] 取消订单" << endl;
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";
	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			string sqlstr = "update `order` set `status`=-1 where oid=" + oid;
			database_conn.Query(sqlstr);
			actiondisplay("取消订单成功！");
			cout << "\t";
			system("pause");
			return;
		}
		actiondisplay("即将取消订单！");
		cout << endl << "\t[1] 取消订单" << endl;
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择项目进行操作：";
	}
}

void actionViewCartItem(string caid) {
	vector<vector<string> > data;
	string sqlstr = "SELECT caid,c.name as catg_name,p.brand,p.name,p.price,ca.quantity,p.pid from cart ca left join product p on ca.pid=p.pid left join category c on c.cid=p.catg_id where caid=" + caid;
	database_conn.Select(sqlstr, data);

	if (data.size() > 0) {

		actiondisplay("购物车商品详情");

		cout << "\t商品名：" << data[0][3] << endl << endl;
		cout << "\t商品分类：" << data[0][1] << endl << endl;
		cout << "\t商品品牌：" << data[0][2] << endl << endl;
		cout << "\t商品单价：￥" << str2money_sync(data[0][4]) << endl << endl;

		cout << "\t购买数量：" << data[0][5] << endl << endl;
		cout << "\t花费总额：￥" << str2money_sync(int2str_sync(str2int_sync(data[0][4])*str2int_sync(data[0][5]))) << endl << endl;

	}
	else {
		actiondisplay("购物车商品未找到");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl;
	cout << "\t[1] 修改数量" << endl;
	cout << "\t[2] 移除本商品" << endl;
	cout << "\t[3] 查看商品详情" << endl;

	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			actionModifyCartItem(caid);
		}
		else if (temp_string == "2") {
			actionRemoveCartItem(caid);
			return;
		}
		else if (temp_string == "3") {
			actionViewProduct(data[0][6]);
		}

		database_conn.Select(sqlstr, data);

		actiondisplay("购物车商品详情");

		cout << "\t商品名：" << data[0][3] << endl << endl;
		cout << "\t商品分类：" << data[0][1] << endl << endl;
		cout << "\t商品品牌：" << data[0][2] << endl << endl;
		cout << "\t商品单价：￥" << str2money_sync(data[0][4]) << endl << endl;

		cout << "\t购买数量：" << data[0][5] << endl << endl;
		cout << "\t花费总额：￥" << str2money_sync(int2str_sync((long long)str2int_sync(data[0][4])*str2int_sync(data[0][5]))) << endl << endl;
		cout << endl;
		cout << "\t[1] 修改数量" << endl;
		cout << "\t[2] 移除本商品" << endl;
		cout << "\t[3] 查看商品详情" << endl;
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

void actionModifyCartItem(string caid) {
	actiondisplay("修改购买数量");

	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请输入一个新的数量：";
	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") return;
		regex pattern("(\\d{1,9})");
		if (regex_match(temp_string, pattern)) {
			string sqlstr = "update cart set quantity=" + temp_string + " where caid=" + caid;
			database_conn.Query(sqlstr);
			actiondisplay("数量修改成功！");
			cout << "\t";
			system("pause");
			return;
		}
		actiondisplay("数量不合法！");
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请输入一个合法的数量：";
	}
}

void actionRemoveCartItem(string caid) {
	string sqlstr = "delete from cart where caid=" + caid;
	database_conn.Query(sqlstr);
	actiondisplay("已经成功移除商品！");
	cout << "\t";
	system("pause");
	return;
}

void actionClearCart() {
	string sqlstr = "delete from cart where uid=" + int2str_sync(userinfo.uid);
	database_conn.Query(sqlstr);
	actiondisplay("已经成功清空购物车！");
	cout << "\t";
	system("pause");
	return;
}

void actionAddItem() {
	string sqlstr = "insert into product set name='未命名新商品',catg_id=1,price=10000,discount_price=10000,brand='未知',description='无',detail='无',extra='无',available=1";
	int pid=database_conn.GetInsertID(sqlstr);
	actiondisplay("新建商品");
	cout << "\t新建商品成功，即将跳转编辑界面，";
	system("pause");
	actionModifyItem(int2str_sync(pid));
	return;
}

void actionModifyItemKey(string pid, string key,string desc) {
	actiondisplay("修改"+desc);

	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请输入字段信息：";
	if (key == "price") cout << "￥";
	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") return;
		regex pattern("(\\d{1,9})");
		if (key == "price") {
			if (!regex_match(temp_string, pattern)) {
				actiondisplay("价格不合法！");
				cout << endl << "\t[0] 返回上一级" << endl;
				cout << endl << "\t----------------" << endl << "\t请输入一个合法的字段信息：￥";
				continue;
			}
		}
		string sqlstr;
		if (key == "catg_name") {
			sqlstr = "select cid from category where where name='" + temp_string+ "'";
			vector<vector<string> > data;
			database_conn.Select(sqlstr, data);
			if (data.size()) {
				sqlstr = "update product set catg_id=" + data[0][0] + " where pid=" + pid;
			}
			else {
				actiondisplay("分类不存在！");
				cout << endl << "\t[0] 返回上一级" << endl;
				cout << endl << "\t----------------" << endl << "\t请输入一个合法的字段信息：";
				continue;
			}
		}
		else if (key == "price") sqlstr = "update product set " + key + "=100*" + temp_string + " where pid=" + pid;
		else sqlstr = "update product set " + key + "='" + temp_string + "' where pid=" + pid;
		database_conn.Query(sqlstr);
		actiondisplay("修改成功！");
		cout << "\t";
		system("pause");
		return;
	}
}

void actionModifyItem(string pid) {
	vector<vector<string> > data;
	string sqlstr = "SELECT pid,c.name as catg_name,p.brand,p.name,p.price,p.description,p.detail,p.extra,p.available from product p left join category c on c.cid=p.catg_id where pid=" + pid + " ";
	database_conn.Select(sqlstr, data);
	if (data.size() > 0) {
		if (data[0][8] == "1") {

			actiondisplay("修改商品");

			cout << "\t[1] 修改商品名：" << data[0][3] << endl << endl;
			cout << "\t[2] 修改商品分类：" << data[0][1] << endl << endl;
			cout << "\t[3] 修改商品品牌：" << data[0][2] << endl << endl;
			cout << "\t[4] 修改商品价格：￥" << str2money_sync(data[0][4]) << endl << endl;
			cout << "\t[5] 修改商品简介：" << data[0][5] << endl << endl;
			cout << "\t[6] 修改商品详情：" << data[0][6] << endl << endl;
			cout << "\t[7] 修改其他信息：" << data[0][7] << endl << endl;

		}
		else {
			actiondisplay("商品已下架");
			cout << "\t";
			system("pause");
			return;
		}

	}
	else {
		actiondisplay("商品未找到");
		cout << "\t";
		system("pause");
		return;
	}
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";

	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") {
			return;
		}
		else if (temp_string == "1") {
			if (userinfo.admin) {
				actionModifyItemKey(pid, "name","商品名");
			}
		}
		else if (temp_string == "2") {
			if (userinfo.admin) {
				actionModifyItemKey(pid, "catg_name", "商品分类");
			}
		}
		else if (temp_string == "3") {
			if (userinfo.admin) {
				actionModifyItemKey(pid, "brand", "商品品牌");
			}
		}
		else if (temp_string == "4") {
			if (userinfo.admin) {
				actionModifyItemKey(pid, "price", "商品价格");
			}
		}
		else if (temp_string == "5") {
			if (userinfo.admin) {
				actionModifyItemKey(pid, "description", "商品简介");
			}
		}
		else if (temp_string == "6") {
			if (userinfo.admin) {
				actionModifyItemKey(pid, "detail", "商品详情");
			}
		}
		else if (temp_string == "7") {
			if (userinfo.admin) {
				actionModifyItemKey(pid, "extra", "其他信息");
			}
		}

		database_conn.Select(sqlstr, data);
		if (data.size() > 0) {
			if (data[0][8] == "1") {

				actiondisplay("修改商品");

				cout << "\t[1] 修改商品名：" << data[0][3] << endl << endl;
				cout << "\t[2] 修改商品分类：" << data[0][1] << endl << endl;
				cout << "\t[3] 修改商品品牌：" << data[0][2] << endl << endl;
				cout << "\t[4] 修改商品价格：￥" << str2money_sync(data[0][4]) << endl << endl;
				cout << "\t[5] 修改商品简介：" << data[0][5] << endl << endl;
				cout << "\t[6] 修改商品详情：" << data[0][6] << endl << endl;
				cout << "\t[7] 修改其他信息：" << data[0][7] << endl << endl;

			}
			else {
				actiondisplay("商品已下架");
				cout << "\t";
				system("pause");
				return;
			}

		}
		else {
			actiondisplay("商品未找到");
			cout << "\t";
			system("pause");
			return;
		}
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

void actionDeleteItem(string pid) {
	string sqlstr = "update product set available=0 where pid=" + pid;
	database_conn.Query(sqlstr);
	sqlstr = "delete from cart where pid=" + pid;
	database_conn.Query(sqlstr);
	actiondisplay("已经成功移除商品！");
	cout << "\t";
	system("pause");
	return;
}

void actionManageItems() {
	actiondisplay("管理商品");
	cout << endl << "\t[1] 新增商品" << endl;
	cout << endl << "\t[0] 返回上一级" << endl;
	cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	string temp_string;
	while (getline(cin, temp_string)) {
		if (temp_string == "0") return;
		else if (temp_string == "1") {
			actionAddItem();
		}
		actiondisplay("管理商品");
		cout << endl << "\t[1] 新增商品" << endl;
		cout << endl << "\t[0] 返回上一级" << endl;
		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
	}
}

bool actionCheckLogin() {
	ifstream f("user.config", ios::in);
	if (!f.is_open())
	{
		ofstream fout("user.config");
		fout.close();
		return false;
	}
	string email, pass;
	f >> email >> pass;
	f.close();
	vector<vector<string> > data;
	string sqlstr = "SELECT uid,name,email,password,admin from users where email='" + email + "' and password='" + pass + "'";
	database_conn.Select(sqlstr, data);

	if (data.size()) {
		str2int(userinfo.uid, data[0][0]);
		userinfo.name = data[0][1];
		userinfo.email = data[0][2];
		userinfo.password = data[0][3];
		userinfo.admin = str2int_sync(data[0][4]);
		return true;
	}
	else {
		return false;
	}
}

int main() {

	SetConsoleTitle(TEXT("淘不了宝 - Kernel Version"));

	HANDLE hMutex = CreateMutex(NULL,FALSE,TEXT("RunOnlyOneInstance"));	//命名Mutex是全局对象     
																		//在所有的process都可以访问到     
	if (hMutex == NULL || ERROR_ALREADY_EXISTS == ::GetLastError()) {
		actiondisplay("请勿重复打开本程序。");
		cout << "\t";
		system("pause");
		return 0;
	}

	ios::sync_with_stdio(false);										//关闭流同步

	char host[] = "localhost";
	char database[] = "comm";
	char user[] = "root";
	char pass[] = "root";

	database_conn.SetMySQLConInfo(host, user, pass, database, 3306);

	if (!database_conn.Open()) {
		cout << database_conn.ErrorNum << " : " << database_conn.ErrorInfo << endl;
		actiondisplay("本程序需要进行一次初始化，请先建立您的MySQL服务器并将comm.sql导入comm数据库，具体配置可在开头更改。");
		cout << "\t";
		system("pause");
		return 0;
	}

	string input_stream;

	if (!actionCheckLogin()) {

		actiondisplay("请先登录以继续操作。");

		cout << "\t[1] 登录" << endl << "\t[2] 注册" << endl;

		cout << endl << "\t----------------" << endl << "\t请选择一个操作：";

		while (getline(cin, input_stream)) {
			if (input_stream == "1") {
				if (actionLogin()) {
					break;
				}
				else {
					actiondisplay("用户名或密码错误！");
					cout << "\t[1] 登录" << endl << "\t[2] 注册" << endl;
					cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
				}
			}
			else if (input_stream == "2") {
				int ret = actionRegister();
				if (ret == 1) {
					break;
				}
				else {
					if (ret == 0) {
						actiondisplay("邮箱已经被使用！");
					}
					else if (ret == -1) {
						actiondisplay("邮箱不合法！");
					}
					else if (ret == -2) {
						actiondisplay("用户名不能为空！");
					}
					else if (ret == -3) {
						actiondisplay("密码不能过短！");
					}
					cout << "\t[1] 登录" << endl << "\t[2] 注册" << endl;
					cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
				}
			}
			else {
				actiondisplay("请先登录以继续操作。");
				cout << "\t[1] 登录" << endl << "\t[2] 注册" << endl;
				cout << endl << "\t----------------" << endl << "\t请选择一个操作：";
			}
		}
	}

	string recommendId=actionShowMenu("0");

	while (getline(cin, input_stream)) {
		if (input_stream == "0"+ recommendId) {
			actionViewProduct(recommendId);
		}
		else if (input_stream == "1") {
			actionSearch();
		}
		else if (input_stream == "2") {
			actionViewCatg();
		}
		else if (input_stream == "3") {
			actionSearchUser();
		}
		else if (input_stream == "4") {
			actionViewOrder();
		}
		else if (input_stream == "5") {
			actionViewCart();
		}
		else if (input_stream == "6") {
			actionViewBalance();
		}
		else if (input_stream == "7") {
			ofstream fout("user.config");
			fout << "" << endl;
			fout.close();
			cout << endl << "\t注销成功，";
			break;
		}
		else if (input_stream == "8") {
			if (userinfo.admin) {
				actionManageItems();
			}
		}
		recommendId = actionShowMenu("0");
	}

	system("pause");

	database_conn.Close();
	return 0;
}