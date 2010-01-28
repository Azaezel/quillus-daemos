class Test
{
    public function new(name : String)
    {
        this.name = name;
    }

    public function printHello()
    {
        php.Lib.print('Hello there, ' + this.name + '!');
    }

    private var name : String;
}
