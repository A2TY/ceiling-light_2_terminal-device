
test() {
    var path = "./test.txt";
    var fileObj = File(path);
    var mode = "w"; // w:書き込みモード r:読み込みモード e:編集モード
    var success = fileObj.open(mode); // 成功すればsuccessはtrueを返す
    fileObj.writeln("いちぎょうかきこみ"); // writelnメソッドで1行ぶん書き込む
    fileObj.close();
}