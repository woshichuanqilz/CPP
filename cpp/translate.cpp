public partial class MainWindow : Window
    {
        string url = "";
        public MainWindow()
        {
            InitializeComponent();
            this.cbxTranslateChange.SelectedIndex = 0;
        }
        private TranClass tranClass = new TranClass();
        private void btnTranslate_Click(object sender, RoutedEventArgs e)
        {

            WebClient client = new WebClient();  //引用System.Net
            string fromTranslate = tbxFromContent.Text; //翻译前的内容
            if (!string.IsNullOrEmpty(fromTranslate))
            {
　　　　　　　　　//client_id为自己的api_id，q为翻译对象，from为翻译语言，to为翻译后语言
                string url = string.Format("http://openapi.baidu.com/public/2.0/bmt/translate?client_id={0}&q={1}&from={2}&to={3}", "youAPI_Id", fromTranslate, tranClass.From, tranClass.To);
                var buffer = client.DownloadData(url);
                string result = Encoding.UTF8.GetString(buffer);
                StringReader sr = new StringReader(result);
                JsonTextReader jsonReader = new JsonTextReader(sr); //引用Newtonsoft.Json 自带
                JsonSerializer serializer = new JsonSerializer();
                var r = serializer.Deserialize<TranClass>(jsonReader); //因为获取后的为json对象 ，实行转换
                tbxToContent.Text = r.Trans_result[0].dst;  //dst为翻译后的值
            }
        }
　　　　　
　　　　　
　　　　　//改变翻译语言来执行相应的操作
        private void cbxTranslateChange_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBox comboBox = sender as ComboBox;
            switch (comboBox.SelectedIndex)
            {
                case 1:
                    tranClass.From = "zh";
                    tranClass.To = "en";
                    break;
                case 2:
                    tranClass.From = "en";
                    tranClass.To = "zh";
                    break;
                case 3:
                    tranClass.From = "zh";
                    tranClass.To = "jp";
                    break;
                case 4:
                    tranClass.From = "jp";
                    tranClass.To = "zh";
                    break;
                case 5:
                    tranClass.From = "zh";
                    tranClass.To = "kor";
                    break;
                case 6:
                    tranClass.From = "kor";
                    tranClass.To = "zh";
                    break;
                case 7:
                    tranClass.From = "zh";
                    tranClass.To = "spa";
                    break;
                case 8:
                    tranClass.From = "spa";
                    tranClass.To = "zh";
                    break;
                default:
                    tranClass.From = "auto";
                    tranClass.To = "auto";
                    break;

            }
        }

　　　　　//对翻译后的内容发音，操作类具体我也不了解，默认发音英文，中文，其他语言要自己添加。
        private void btnsound_Click(object sender, RoutedEventArgs e)
        {
            ///gettts?lan=en&text=Scattering&spd=2&source=web
            SpeechLib.SpeechVoiceSpeakFlags svsFlags = SpeechLib.SpeechVoiceSpeakFlags.SVSFlagsAsync;
            SpeechLib.SpVoice voice = new SpeechLib.SpVoice();
            voice.Speak(tbxToContent.Text, svsFlags);

        }
    }

这是返回结果使用的类

namespace BDTranslate.Models
{
    public class TranClass
    {
        public string From { get; set; }
        public string To { get; set; }
        public List<Trans_result> Trans_result { get; set; }
    }
    public class Trans_result
    {
        public string src { get; set; }
        public string dst { get; set; }
    }
}

 百度翻译api ：http://developer.baidu.com/wiki/index.php?title=帮助文档首页/百度翻译/翻译API

C# 调用百度翻译Api

