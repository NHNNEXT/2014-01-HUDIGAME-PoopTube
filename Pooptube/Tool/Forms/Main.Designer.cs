namespace Tool
{
    partial class MainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.ViewBox = new System.Windows.Forms.PictureBox();
            this.MainMenu = new System.Windows.Forms.MenuStrip();
            this.파일fToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.NewMapMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenMapMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.CloseMapMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.SaveMapMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveAsMapMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.ExitMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.편집EToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.UndoMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.RedoMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.CutMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.CopyMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.PasteMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.DeleteMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.SelectAllMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.보기VToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.빌드BToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.도구TToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.창WToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.도움말HToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OptionTab = new System.Windows.Forms.TabControl();
            this.PropertyTab = new System.Windows.Forms.TabPage();
            this.ObjectListBox = new System.Windows.Forms.ComboBox();
            this.PropertyForm = new System.Windows.Forms.PropertyGrid();
            this.ComponentTab = new System.Windows.Forms.TabPage();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.ActionTab = new System.Windows.Forms.TabPage();
            this.ObjectRadioTree3 = new System.Windows.Forms.RadioButton();
            this.ObjectRadioTree2 = new System.Windows.Forms.RadioButton();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.GroundTUp = new System.Windows.Forms.RadioButton();
            this.GroundTDown = new System.Windows.Forms.RadioButton();
            this.ObjectRadioCreature = new System.Windows.Forms.RadioButton();
            this.ObjectRadioTree1 = new System.Windows.Forms.RadioButton();
            this.ObjectRadioLightOrb = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.ObjectRadioBush = new System.Windows.Forms.RadioButton();
            ((System.ComponentModel.ISupportInitialize)(this.ViewBox)).BeginInit();
            this.MainMenu.SuspendLayout();
            this.OptionTab.SuspendLayout();
            this.PropertyTab.SuspendLayout();
            this.ComponentTab.SuspendLayout();
            this.ActionTab.SuspendLayout();
            this.SuspendLayout();
            // 
            // ViewBox
            // 
            this.ViewBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ViewBox.Location = new System.Drawing.Point(12, 27);
            this.ViewBox.Name = "ViewBox";
            this.ViewBox.Size = new System.Drawing.Size(473, 304);
            this.ViewBox.TabIndex = 1;
            this.ViewBox.TabStop = false;
            this.ViewBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ViewBox_MouseDown);
            // 
            // MainMenu
            // 
            this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.파일fToolStripMenuItem,
            this.편집EToolStripMenuItem,
            this.보기VToolStripMenuItem,
            this.빌드BToolStripMenuItem,
            this.도구TToolStripMenuItem,
            this.창WToolStripMenuItem,
            this.도움말HToolStripMenuItem});
            this.MainMenu.Location = new System.Drawing.Point(0, 0);
            this.MainMenu.Name = "MainMenu";
            this.MainMenu.Size = new System.Drawing.Size(753, 24);
            this.MainMenu.TabIndex = 2;
            this.MainMenu.Text = "menuStrip1";
            // 
            // 파일fToolStripMenuItem
            // 
            this.파일fToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.NewMapMenu,
            this.OpenMapMenu,
            this.toolStripSeparator1,
            this.CloseMapMenu,
            this.toolStripSeparator2,
            this.SaveMapMenu,
            this.SaveAsMapMenu,
            this.toolStripSeparator3,
            this.ExitMenu});
            this.파일fToolStripMenuItem.Name = "파일fToolStripMenuItem";
            this.파일fToolStripMenuItem.Size = new System.Drawing.Size(58, 20);
            this.파일fToolStripMenuItem.Text = "파일(&F)";
            // 
            // NewMapMenu
            // 
            this.NewMapMenu.Name = "NewMapMenu";
            this.NewMapMenu.Size = new System.Drawing.Size(192, 22);
            this.NewMapMenu.Text = "새로 만들기(N)";
            this.NewMapMenu.Click += new System.EventHandler(this.NewMapFile);
            // 
            // OpenMapMenu
            // 
            this.OpenMapMenu.Name = "OpenMapMenu";
            this.OpenMapMenu.Size = new System.Drawing.Size(192, 22);
            this.OpenMapMenu.Text = "열기(O)";
            this.OpenMapMenu.Click += new System.EventHandler(this.LoadMapFile);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(189, 6);
            // 
            // CloseMapMenu
            // 
            this.CloseMapMenu.Name = "CloseMapMenu";
            this.CloseMapMenu.Size = new System.Drawing.Size(192, 22);
            this.CloseMapMenu.Text = "닫기(C)";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(189, 6);
            // 
            // SaveMapMenu
            // 
            this.SaveMapMenu.Name = "SaveMapMenu";
            this.SaveMapMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.SaveMapMenu.Size = new System.Drawing.Size(192, 22);
            this.SaveMapMenu.Text = "저장(S)";
            this.SaveMapMenu.Click += new System.EventHandler(this.SaveMapFile);
            // 
            // SaveAsMapMenu
            // 
            this.SaveAsMapMenu.Name = "SaveAsMapMenu";
            this.SaveAsMapMenu.Size = new System.Drawing.Size(192, 22);
            this.SaveAsMapMenu.Text = "다른 이름으로 저장(A)";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(189, 6);
            // 
            // ExitMenu
            // 
            this.ExitMenu.Name = "ExitMenu";
            this.ExitMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.ExitMenu.Size = new System.Drawing.Size(192, 22);
            this.ExitMenu.Text = "끝내기(X)";
            this.ExitMenu.Click += new System.EventHandler(this.ExitProgram);
            // 
            // 편집EToolStripMenuItem
            // 
            this.편집EToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.UndoMenu,
            this.RedoMenu,
            this.toolStripSeparator4,
            this.CutMenu,
            this.CopyMenu,
            this.PasteMenu,
            this.DeleteMenu,
            this.toolStripSeparator5,
            this.SelectAllMenu});
            this.편집EToolStripMenuItem.Name = "편집EToolStripMenuItem";
            this.편집EToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
            this.편집EToolStripMenuItem.Text = "편집(&E)";
            // 
            // UndoMenu
            // 
            this.UndoMenu.Name = "UndoMenu";
            this.UndoMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.UndoMenu.Size = new System.Drawing.Size(178, 22);
            this.UndoMenu.Text = "실행 취소(U)";
            // 
            // RedoMenu
            // 
            this.RedoMenu.Name = "RedoMenu";
            this.RedoMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.RedoMenu.Size = new System.Drawing.Size(178, 22);
            this.RedoMenu.Text = "다시 실행(R)";
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(175, 6);
            // 
            // CutMenu
            // 
            this.CutMenu.Name = "CutMenu";
            this.CutMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.CutMenu.Size = new System.Drawing.Size(178, 22);
            this.CutMenu.Text = "잘라내기(X)";
            // 
            // CopyMenu
            // 
            this.CopyMenu.Name = "CopyMenu";
            this.CopyMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.CopyMenu.Size = new System.Drawing.Size(178, 22);
            this.CopyMenu.Text = "복사(C)";
            // 
            // PasteMenu
            // 
            this.PasteMenu.Name = "PasteMenu";
            this.PasteMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.PasteMenu.Size = new System.Drawing.Size(178, 22);
            this.PasteMenu.Text = "붙여넣기(P)";
            // 
            // DeleteMenu
            // 
            this.DeleteMenu.Name = "DeleteMenu";
            this.DeleteMenu.ShortcutKeys = System.Windows.Forms.Keys.Delete;
            this.DeleteMenu.Size = new System.Drawing.Size(178, 22);
            this.DeleteMenu.Text = "삭제(D)";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(175, 6);
            // 
            // SelectAllMenu
            // 
            this.SelectAllMenu.Name = "SelectAllMenu";
            this.SelectAllMenu.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.SelectAllMenu.Size = new System.Drawing.Size(178, 22);
            this.SelectAllMenu.Text = "모두 선택(A)";
            // 
            // 보기VToolStripMenuItem
            // 
            this.보기VToolStripMenuItem.Name = "보기VToolStripMenuItem";
            this.보기VToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
            this.보기VToolStripMenuItem.Text = "보기(&V)";
            // 
            // 빌드BToolStripMenuItem
            // 
            this.빌드BToolStripMenuItem.Name = "빌드BToolStripMenuItem";
            this.빌드BToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
            this.빌드BToolStripMenuItem.Text = "빌드(&B)";
            // 
            // 도구TToolStripMenuItem
            // 
            this.도구TToolStripMenuItem.Name = "도구TToolStripMenuItem";
            this.도구TToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
            this.도구TToolStripMenuItem.Text = "도구(&T)";
            // 
            // 창WToolStripMenuItem
            // 
            this.창WToolStripMenuItem.Name = "창WToolStripMenuItem";
            this.창WToolStripMenuItem.Size = new System.Drawing.Size(49, 20);
            this.창WToolStripMenuItem.Text = "창(&W)";
            // 
            // 도움말HToolStripMenuItem
            // 
            this.도움말HToolStripMenuItem.Name = "도움말HToolStripMenuItem";
            this.도움말HToolStripMenuItem.Size = new System.Drawing.Size(71, 20);
            this.도움말HToolStripMenuItem.Text = "도움말(&H)";
            // 
            // OptionTab
            // 
            this.OptionTab.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.OptionTab.Controls.Add(this.PropertyTab);
            this.OptionTab.Controls.Add(this.ComponentTab);
            this.OptionTab.Controls.Add(this.ActionTab);
            this.OptionTab.Location = new System.Drawing.Point(491, 27);
            this.OptionTab.Name = "OptionTab";
            this.OptionTab.SelectedIndex = 0;
            this.OptionTab.Size = new System.Drawing.Size(262, 304);
            this.OptionTab.TabIndex = 3;
            // 
            // PropertyTab
            // 
            this.PropertyTab.Controls.Add(this.ObjectListBox);
            this.PropertyTab.Controls.Add(this.PropertyForm);
            this.PropertyTab.Location = new System.Drawing.Point(4, 22);
            this.PropertyTab.Name = "PropertyTab";
            this.PropertyTab.Padding = new System.Windows.Forms.Padding(3);
            this.PropertyTab.Size = new System.Drawing.Size(254, 278);
            this.PropertyTab.TabIndex = 0;
            this.PropertyTab.Text = "Property";
            this.PropertyTab.UseVisualStyleBackColor = true;
            // 
            // ObjectListBox
            // 
            this.ObjectListBox.Dock = System.Windows.Forms.DockStyle.Top;
            this.ObjectListBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ObjectListBox.FormattingEnabled = true;
            this.ObjectListBox.Location = new System.Drawing.Point(3, 3);
            this.ObjectListBox.Margin = new System.Windows.Forms.Padding(0);
            this.ObjectListBox.Name = "ObjectListBox";
            this.ObjectListBox.Size = new System.Drawing.Size(248, 20);
            this.ObjectListBox.TabIndex = 4;
            // 
            // PropertyForm
            // 
            this.PropertyForm.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PropertyForm.HelpVisible = false;
            this.PropertyForm.Location = new System.Drawing.Point(3, 23);
            this.PropertyForm.Margin = new System.Windows.Forms.Padding(0);
            this.PropertyForm.Name = "PropertyForm";
            this.PropertyForm.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.PropertyForm.Size = new System.Drawing.Size(248, 252);
            this.PropertyForm.TabIndex = 0;
            this.PropertyForm.ToolbarVisible = false;
            this.PropertyForm.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.PropertyValueChanged);
            // 
            // ComponentTab
            // 
            this.ComponentTab.Controls.Add(this.richTextBox1);
            this.ComponentTab.Location = new System.Drawing.Point(4, 22);
            this.ComponentTab.Name = "ComponentTab";
            this.ComponentTab.Padding = new System.Windows.Forms.Padding(3);
            this.ComponentTab.Size = new System.Drawing.Size(254, 278);
            this.ComponentTab.TabIndex = 1;
            this.ComponentTab.Text = "Component";
            this.ComponentTab.UseVisualStyleBackColor = true;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(6, 3);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(242, 248);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // ActionTab
            // 
            this.ActionTab.Controls.Add(this.ObjectRadioBush);
            this.ActionTab.Controls.Add(this.ObjectRadioTree3);
            this.ActionTab.Controls.Add(this.ObjectRadioTree2);
            this.ActionTab.Controls.Add(this.label5);
            this.ActionTab.Controls.Add(this.label4);
            this.ActionTab.Controls.Add(this.GroundTUp);
            this.ActionTab.Controls.Add(this.GroundTDown);
            this.ActionTab.Controls.Add(this.ObjectRadioCreature);
            this.ActionTab.Controls.Add(this.ObjectRadioTree1);
            this.ActionTab.Controls.Add(this.ObjectRadioLightOrb);
            this.ActionTab.Location = new System.Drawing.Point(4, 22);
            this.ActionTab.Name = "ActionTab";
            this.ActionTab.Padding = new System.Windows.Forms.Padding(3);
            this.ActionTab.Size = new System.Drawing.Size(254, 278);
            this.ActionTab.TabIndex = 2;
            this.ActionTab.Text = "Action";
            this.ActionTab.UseVisualStyleBackColor = true;
            this.ActionTab.Click += new System.EventHandler(this.ActionTab_Click);
            // 
            // ObjectRadioTree3
            // 
            this.ObjectRadioTree3.AutoSize = true;
            this.ObjectRadioTree3.Location = new System.Drawing.Point(160, 63);
            this.ObjectRadioTree3.Name = "ObjectRadioTree3";
            this.ObjectRadioTree3.Size = new System.Drawing.Size(55, 16);
            this.ObjectRadioTree3.TabIndex = 6;
            this.ObjectRadioTree3.Text = "Tree3";
            this.ObjectRadioTree3.UseVisualStyleBackColor = true;
            this.ObjectRadioTree3.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // ObjectRadioTree2
            // 
            this.ObjectRadioTree2.AutoSize = true;
            this.ObjectRadioTree2.Location = new System.Drawing.Point(82, 63);
            this.ObjectRadioTree2.Name = "ObjectRadioTree2";
            this.ObjectRadioTree2.Size = new System.Drawing.Size(55, 16);
            this.ObjectRadioTree2.TabIndex = 5;
            this.ObjectRadioTree2.Text = "Tree2";
            this.ObjectRadioTree2.UseVisualStyleBackColor = true;
            this.ObjectRadioTree2.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 109);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(122, 12);
            this.label5.TabIndex = 4;
            this.label5.Text = "Ground Terraforming";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 3);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 3;
            this.label4.Text = "Object";
            // 
            // GroundTUp
            // 
            this.GroundTUp.AutoSize = true;
            this.GroundTUp.Location = new System.Drawing.Point(8, 124);
            this.GroundTUp.Name = "GroundTUp";
            this.GroundTUp.Size = new System.Drawing.Size(38, 16);
            this.GroundTUp.TabIndex = 0;
            this.GroundTUp.Text = "Up";
            this.GroundTUp.UseVisualStyleBackColor = true;
            this.GroundTUp.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // GroundTDown
            // 
            this.GroundTDown.AutoSize = true;
            this.GroundTDown.Location = new System.Drawing.Point(8, 146);
            this.GroundTDown.Name = "GroundTDown";
            this.GroundTDown.Size = new System.Drawing.Size(55, 16);
            this.GroundTDown.TabIndex = 1;
            this.GroundTDown.Text = "Down";
            this.GroundTDown.UseVisualStyleBackColor = true;
            this.GroundTDown.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // ObjectRadioCreature
            // 
            this.ObjectRadioCreature.AutoSize = true;
            this.ObjectRadioCreature.Checked = true;
            this.ObjectRadioCreature.Location = new System.Drawing.Point(8, 19);
            this.ObjectRadioCreature.Name = "ObjectRadioCreature";
            this.ObjectRadioCreature.Size = new System.Drawing.Size(71, 16);
            this.ObjectRadioCreature.TabIndex = 0;
            this.ObjectRadioCreature.TabStop = true;
            this.ObjectRadioCreature.Text = "Creature";
            this.ObjectRadioCreature.UseVisualStyleBackColor = true;
            this.ObjectRadioCreature.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // ObjectRadioTree1
            // 
            this.ObjectRadioTree1.AutoSize = true;
            this.ObjectRadioTree1.Location = new System.Drawing.Point(8, 63);
            this.ObjectRadioTree1.Name = "ObjectRadioTree1";
            this.ObjectRadioTree1.Size = new System.Drawing.Size(55, 16);
            this.ObjectRadioTree1.TabIndex = 2;
            this.ObjectRadioTree1.Text = "Tree1";
            this.ObjectRadioTree1.UseVisualStyleBackColor = true;
            this.ObjectRadioTree1.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // ObjectRadioLightOrb
            // 
            this.ObjectRadioLightOrb.AutoSize = true;
            this.ObjectRadioLightOrb.Location = new System.Drawing.Point(8, 41);
            this.ObjectRadioLightOrb.Name = "ObjectRadioLightOrb";
            this.ObjectRadioLightOrb.Size = new System.Drawing.Size(90, 16);
            this.ObjectRadioLightOrb.TabIndex = 1;
            this.ObjectRadioLightOrb.Text = "Light Object";
            this.ObjectRadioLightOrb.UseVisualStyleBackColor = true;
            this.ObjectRadioLightOrb.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(535, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "label1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(594, 12);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "label2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(662, 12);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "label3";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(426, 1);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 7;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // ObjectRadioBush
            // 
            this.ObjectRadioBush.AutoSize = true;
            this.ObjectRadioBush.Location = new System.Drawing.Point(8, 85);
            this.ObjectRadioBush.Name = "ObjectRadioBush";
            this.ObjectRadioBush.Size = new System.Drawing.Size(52, 16);
            this.ObjectRadioBush.TabIndex = 7;
            this.ObjectRadioBush.Text = "Bush";
            this.ObjectRadioBush.UseVisualStyleBackColor = true;
            this.ObjectRadioBush.CheckedChanged += new System.EventHandler(this.ChangedObjectType);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(753, 343);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.OptionTab);
            this.Controls.Add(this.ViewBox);
            this.Controls.Add(this.MainMenu);
            this.KeyPreview = true;
            this.MainMenuStrip = this.MainMenu;
            this.Name = "MainForm";
            this.Text = "mastrayer";
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyboardInput);
            ((System.ComponentModel.ISupportInitialize)(this.ViewBox)).EndInit();
            this.MainMenu.ResumeLayout(false);
            this.MainMenu.PerformLayout();
            this.OptionTab.ResumeLayout(false);
            this.PropertyTab.ResumeLayout(false);
            this.ComponentTab.ResumeLayout(false);
            this.ActionTab.ResumeLayout(false);
            this.ActionTab.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox ViewBox;
        private System.Windows.Forms.MenuStrip MainMenu;
        private System.Windows.Forms.ToolStripMenuItem 파일fToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 편집EToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 보기VToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 빌드BToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 도구TToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 창WToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 도움말HToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem NewMapMenu;
        private System.Windows.Forms.ToolStripMenuItem OpenMapMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem CloseMapMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem SaveMapMenu;
        private System.Windows.Forms.ToolStripMenuItem SaveAsMapMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem ExitMenu;
        private System.Windows.Forms.ToolStripMenuItem UndoMenu;
        private System.Windows.Forms.ToolStripMenuItem RedoMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem CutMenu;
        private System.Windows.Forms.ToolStripMenuItem CopyMenu;
        private System.Windows.Forms.ToolStripMenuItem PasteMenu;
        private System.Windows.Forms.ToolStripMenuItem DeleteMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem SelectAllMenu;
        private System.Windows.Forms.TabControl OptionTab;
        private System.Windows.Forms.TabPage PropertyTab;
        private System.Windows.Forms.TabPage ComponentTab;
        private System.Windows.Forms.PropertyGrid PropertyForm;
        private System.Windows.Forms.ComboBox ObjectListBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TabPage ActionTab;
        private System.Windows.Forms.RadioButton ObjectRadioTree1;
        private System.Windows.Forms.RadioButton ObjectRadioLightOrb;
        private System.Windows.Forms.RadioButton ObjectRadioCreature;
        private System.Windows.Forms.RadioButton GroundTUp;
        private System.Windows.Forms.RadioButton GroundTDown;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.RadioButton ObjectRadioTree3;
        private System.Windows.Forms.RadioButton ObjectRadioTree2;
        private System.Windows.Forms.RadioButton ObjectRadioBush;
    }
}

