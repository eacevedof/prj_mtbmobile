function isNifValido(sNif) 
{
    sNif = sNif.replace("-", "");
    sNif = sNif.toUpperCase();

    var sLetra = 'TRWAGMYFPDXBNJZSQVHLCKET';
    var iNumero = sNif.substr( 0, sNif.length-1);
    var cCaracter = sNif.substr(sNif.length-1, 1);

    iNumero = iNumero % 23;
    sLetra = sLetra.substring(iNumero, iNumero + 1);

    //bug(sLetra+'='+cCaracter);
    if(sLetra != cCaracter) 
    {
        return false;
    }
    return true;
}

function isCifValido(sCif)
{
    sCif = sCif.replace("-", "");
    //http://snipplr.com/view/2609/validar-nifcifdni/
    //sCif: 7 números y un último dígito de control que puede ser o bien un número, o bien una letra 
    var iSumaPares = 0;
    var iSumaImpares = 0;
    var sLetras = "ABCDEFGHKLMNPQS";

    var sCifLetra = sCif.charAt(0);

    if(sCif.length != 9) 
    {
        //bug('El Cif debe tener 9 dígitos');
        return false;
    }

    if (sLetras.indexOf(sCifLetra.toUpperCase()) == -1)
    {
        //bug("El comienzo del Cif no es válido");
        return false;
    }

    //Se suma los valores en posiciones pares
    for(var i=2; i<8; i+=2) 
    {
        iSumaPares = iSumaPares + parseInt(sCif.charAt(i));
    }

    //Multiplicar cada número de las posiciones impares por 2 y en caso de que el número 
    //resultante tenga dos dígitos (>9) sumar estos entre si, por ejemplo si es 8, 8 * 2 = 16, 1 + 6 = 7
    for(i=1; i<9; i+=2) 
    {
        var iAuxImpar = 2 * parseInt(sCif.charAt(i));
        if (iAuxImpar > 9) 
        {
            iAuxImpar = 1 + (iAuxImpar - 10);
        }
        iSumaImpares = iSumaImpares + iAuxImpar;
    }

    //Sumar los resultados de los números impares con los pares y coger de esta suma s�lo el último dígito, por //ejemplo si la suma ha dado 26, se coger�a 6 
    var iSumaParesImpares = iSumaPares + iSumaImpares;
    var iUltimoDigitoSuma = iSumaParesImpares % 10;
    /*
    restar a 10 este número, por ejemplo 10 - 6 = 4. Este último número ser�a el que nos dir�a el valor del último dígito, en este ejemplo, el último dígito podr�a ser o bien este número o bien la letra correspondiente, as� la 'A' corresponde al número '1', la 'B' al '2', y as� sucesivamente. La 'A' corresponde al número '0'
    */	
    var cUltimoDigito = 10 - iUltimoDigitoSuma;
    if (cUltimoDigito == 10) 
    {
        cUltimoDigito = 0;
    } 

    if (cUltimoDigito != sCif.charAt(8)) 
    {
        //alert("El Cif no es válido");
        return false;
    }
    //alert("El Cif es válido");
    return true;
}

function isNieValido(sNie)
{
    sNie = sNie.replace("-","");
    var sNieUpper = sNie.toUpperCase();
    var sLetrasValidas = "TRWAGMYFPDXBNJZSQVHLCKET";
    var iPosicionLetra = 0;
    var sLetraCorresponde =" ";

    //Residente en Espa�a	
    if (sNie.length==9)
    {
        if (sNieUpper.substr(0,1)=="X")
        {
            var iNumerosNie = sNieUpper.substr(1,7);
            /*Resto de la division entre 23 es la posicion en la cadena*/
            iPosicionLetra = iNumerosNie % 23; 
            sLetraCorresponde = sLetrasValidas.substring(iPosicionLetra, iPosicionLetra+1);
            //No cumple con la longitud
            if (!/^[A-Za-z0-9]{9}$/.test(sNieUpper))
            { 
                return false;
            }
            //Cumple con la longitud
            else 
            { 
                //Tiene los 9 dígitos, comprobamos si la letra esta bien
                iNumerosNie = sNieUpper.substr(1,7);
                /*Resto de la division entre 23 es la posicion en la cadena*/
                iPosicionLetra = iNumerosNie % 23; 
                sLetraCorresponde = sLetrasValidas.charAt(iPosicionLetra);
                var sLetraProporcionada = sNieUpper.charAt(8);
                if (sLetraCorresponde != sLetraProporcionada)
                {			
                    return false;
                }				
            }
        }
        //Primera letra distinta de X
        else
        {
            return false;
        }		
    }
    //Numero de digitos mayor a 9
    else
    {
        return false;		
    }
    return true;
}

function is_cif_nif_valido(eText)
{
    //ARCHIVO: nif_cif_v3.js
    var sCifNif = eText.value;
    var isNifOk = isNifValido(sCifNif);
    var isCifOk = isCifValido(sCifNif);
    var isNieOk = isNieValido(sCifNif);

    //bug(isNifOk);bug(isCifOk);bug(isCifOk);
    //Codigo_Sic es el nombre del campo quitando info_
    if(!isNifOk && !isCifOk && !isNieOk && sCifNif != '')
    {
        //bug('Error en dni');
        alert('CIF/NIF inválido');
        //eText.focus();
        jQuery(eText).focus();
    }
}
