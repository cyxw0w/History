using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KuTalkApp
{
    public partial class MainForm : Form
    {
        bool On;
        Point Pos;

        List<FriendInfo> friendsList = new List<FriendInfo>();
        
        public string UserId { get; set; }
        public LoginForm LF { get; set; }

        public MainForm(string uesrid)
        {
            InitializeComponent();

            UserId = uesrid;
        }


        public MainForm(LoginForm loginForm, string uesrid)
        {
            InitializeComponent();

            UserId = uesrid;
            LF = loginForm;
        }

        public void SetListener(LoginForm loginForm)
        {
            LF = loginForm;
        }

        private void MainForm_Load(object sender, EventArgs ex)
        {
            MouseDown += (o, e) => { if (e.Button == MouseButtons.Left) { On = true; Pos = e.Location; } };
            MouseMove += (o, e) => { if (On) Location = new Point(Location.X + (e.X - Pos.X), Location.Y + (e.Y - Pos.Y)); };
            MouseUp += (o, e) => { if (e.Button == MouseButtons.Left) { On = false; Pos = e.Location; } };


            //splitContainer1.Panel1.
            splitContainer1.Panel1.MouseDown += (o, e) => { if (e.Button == MouseButtons.Left) { On = true; Pos = e.Location; } };
            splitContainer1.Panel1.MouseMove += (o, e) => { if (On) Location = new Point(Location.X + (e.X - Pos.X), Location.Y + (e.Y - Pos.Y)); };
            splitContainer1.Panel1.MouseUp += (o, e) => { if (e.Button == MouseButtons.Left) { On = false; Pos = e.Location; } };


            //splitContainer2.Panel1.
            splitContainer2.Panel1.MouseDown += (o, e) => { if (e.Button == MouseButtons.Left) { On = true; Pos = e.Location; } };
            splitContainer2.Panel1.MouseMove += (o, e) => { if (On) Location = new Point(Location.X + (e.X - Pos.X), Location.Y + (e.Y - Pos.Y)); };
            splitContainer2.Panel1.MouseUp += (o, e) => { if (e.Button == MouseButtons.Left) { On = false; Pos = e.Location; } };


            if(LoadFriendInfo())
            {
                PrintFriendInfo();
            }
            else
            {
                MessageBox.Show("친구목록 조회 실패");
                LF.Show();
                this.Dispose();
                return;
            }

            listView1.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
            listView1.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
        }

        private void PrintFriendInfo()
        {
            //int listCnt = friendsList.Count;

            //for (int i = 0; i < friendsList.Count; i++)
            //{
            //    ListViewItem item = new ListViewItem(friendsList[i].FriendName, 0);
            //    item.SubItems.Add(friendsList[i].StatusMsg);

            //    listView1.Items.Add(item);
            //}

            foreach (FriendInfo fi in friendsList)
            {
                //FriendInfo타입의 멤버
                //fi.FriendName;
                //fi.ImgUrl;
                //fi.StatusMsg;
                ListViewItem item = new ListViewItem(fi.FriendName, 0);
                item.SubItems.Add(fi.StatusMsg);

                listView1.Items.Add(item);
            }
        }

        private bool LoadFriendInfo()
        {
            // DB조회
            using (MySqlConnection conn = new MySqlConnection(GVar.mysql_conn_str))
            {
                try
                {
                    conn.Open();

                   
                    string sql2 = "SELECT * FROM tb_friend_list WHERE userid = '" + UserId + "'";


                    MySqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = sql2;

                    MySqlDataReader reader = cmd.ExecuteReader();
                    while (reader.Read())
                    {
                        FriendInfo fi = new FriendInfo();

                        fi.FriendName = reader["friend_userid"].ToString();
                        fi.ImgUrl = reader["friend_img_path"].ToString();
                        fi.StatusMsg = reader["friend_status_msg"].ToString();

                        friendsList.Add(fi);
                    }
                   
                    conn.Close();
                    return true;
                }
                catch (Exception ex)
                {
                    //MessageBox.Show("데이터 베이스 오픈 실패: " + ex.Message, "Database Error[MYSQL]");
                    return false;
                }
                finally
                {
                    //conn.Close();
                }
            }          


        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.Hide();
        }
    }
}
