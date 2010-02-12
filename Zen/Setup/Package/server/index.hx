import Test;
import methods.Branch;

class Index 
{
    static function main() 
    {
        var params = php.Web.getParams();
        var methodName = params.exists('method') ? params.get('method') : null;
        
        if (methodName != null)
        {
            var firstPart : String = methodName.charAt(0).toUpperCase();
            var secondPart : String = methodName.substr(1).toLowerCase();
            var upperCamelCase = firstPart + secondPart;

            var methodClass = Type.resolveClass("methods." + upperCamelCase);
            
            if (methodClass != null)
            {
                var methodParms = new Array();
                var method = Type.createInstance(methodClass, methodParms);
                method.run(params);
            }
            else
            {
                php.Lib.print("Error, invalid method: " + upperCamelCase);
            }
        }
        else
        {
            php.Lib.print("Error, method cannot be empty.");
        }
    }
}
