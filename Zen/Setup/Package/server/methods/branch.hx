package methods;

class Branch
{
    public function run(params : Hash<String>)
    {
        var name = params.exists('name') ? params.get('name') : null;
        php.Lib.print('Branching for ' + name);
    }
}
