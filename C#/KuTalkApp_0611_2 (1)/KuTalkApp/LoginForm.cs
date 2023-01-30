using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KuTalkApp
{
    public partial class LoginForm : Form
    {
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal,
                                                        int size, string filePath);

        private string UserId { set; get; }
        private string UserPw { set; get; }

        bool On;
        Point Pos;        

        public LoginForm()
        {
            InitializeComponent();

            // 폼 로드 후, 
            comboBox1.Focus();
            comboBox1.Select();
            
            UserId = "";
            UserPw = "";

            MouseDown += (o, e) => { if (e.Button == MouseButtons.Left) { On = true; Pos = e.Location; } };
            MouseMove += (o, e) => { if (On) Location = new Point(Location.X + (e.X - Pos.X), Location.Y + (e.Y - Pos.Y)); };
            MouseUp += (o, e) => { if (e.Button == MouseButtons.Left) { On = false; Pos = e.Location; } };

            AcceptButton = button1;


            // DB조회
            //string iniPath = System.Environment.CurrentDirectory + "\\KuTalkInfo.ini";
            //StringBuilder retVal = new StringBuilder();
            //GetPrivateProfileString("Account", "id", "내용없음", retVal, 32, iniPath);
            //UserId = retVal.ToString();
            //GetPrivateProfileString("Account", "password", "내용없음", retVal, 32, iniPath);
            //UserPw = retVal.ToString();

            //WritePrivateProfileString("Account", "id", "test@korea.ac.kr", iniPath);
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            string url = "https://accounts.kakao.com/weblogin/find_account_guide?continue=https%3A%2F%2Faccounts.kakao.com%2Fweblogin%2Faccount%2Finfo";
            System.Diagnostics.Process.Start(url);
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            string url = "https://accounts.kakao.com/weblogin/find_password?continue=https%3A%2F%2Faccounts.kakao.com%2Fweblogin%2Faccount%2Finfo";
            System.Diagnostics.Process.Start(url);
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            int len = textBox2.Text.Length;

            if(len > 3)
            {
                button1.Enabled = true;
                button1.BackColor = Color.FromArgb(66,54,48);
                button1.ForeColor = Color.White;
            }
            else
            {
                button1.Enabled = false;
                button1.BackColor = SystemColors.Control;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            

            string id = comboBox1.Text.ToString();
            string pw = textBox2.Text.ToString();
            int cnt = 0;

            bool success = AuthCheck(id, pw, ref cnt);
            if(success)         // 로그인 성공
            {
                //MessageBox.Show("로그인되었습니다.");

                this.Hide();

                MainForm form = new MainForm(id);
                form.SetListener(this);

                form.Show();

            }
            else                // 로그인 실패
            {
                string msg = "카카오계정 또는 비밀번호를 다시 확인해 주세요.";
                label1.Text = msg;
                textBox2.Text = "";
            }
           
        }

        public bool AuthCheck(string id, string pw, ref int cnt)
        {
            bool bret = true;

            string readPw;

            using (MySqlConnection conn = new MySqlConnection(GVar.mysql_conn_str))
            {
                try
                {
                    conn.Open();

                    //string sql = "Select count(*) as cnt from tb_account where userid = '" + id + "' and userpassword = '" + pw + "'";

                    string sql2 = "SELECT AES_DECRYPT(UNHEX(userpassword), '" +
                        GVar.dec_key +
                        "') as userpassword FROM tb_account WHERE userid = '" +
                        id +
                        "'";


                    MySqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = sql2;

                    MySqlDataReader reader = cmd.ExecuteReader();
                    reader.Read();

                    //cnt = int.Parse(reader["cnt"].ToString());

                    readPw = Encoding.Default.GetString((System.Byte[])reader["userpassword"]);

                    reader.Close();
                    //conn.Close();

                    if (readPw.CompareTo(pw) == 0)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }


                }
                catch (Exception ex)
                {
                    //MessageBox.Show("데이터 베이스 오픈 실패: " + ex.Message, "Database Error[MYSQL]");
                    bret = false;
                }
                finally
                {
                    //conn.Close();
                }
            }

            return bret;
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            FormClose();
        }

        private void LoginForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            FormClose();
        }

        private void FormClose()
        {
            Application.Exit();
            Environment.Exit(0);
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }
    }
}
