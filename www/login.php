<!DOCTYPE HTML>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.0;">
        <script type="text/javascript" src="./js/jquery/jquery-1.7.1.min.js"></script>
        <script type="text/javascript" src="./js/jquery/jquery-ui-1.8.16.tel_mob.min.js"></script>
        <script type="text/javascript" src="./js/jquery/jquery.mobile-1.0.1.min.js"></script>
        <link rel="stylesheet" href="./styles/jquery/jquery-ui-1.8.16.tel_mob.css" type="text/css">
        <link rel="stylesheet" href="./styles/jquery/jquery.mobile-1.0.css" type="text/css">
        <link rel="stylesheet" href="./styles/custom/css_mtbkey_jqmobile.css" type="text/css">
        <title><?php echo "Telynet Mobile"; ?></title>
    </head>
    <body>
        <div data-role="page" id="divPage">
            <div data-role="header" class="ui-bar ui-bar-b" data-theme="b" style="text-align:center;">
                <h3>MT Business Key</h3> 
                <span style="font-size: 10px;">[Mobile Access]</span>
            </div>
            <div data-role="content" id="divContent"> 
                <form name="frmLogin" id="form-login" action="index.php" method="post" data-ajax="false">
                    <div class="ui-btn ui-btn-corner-all ui-shadow ui-btn-up-b" data-theme="b" disabled="true">
                        <span class="ui-btn-inner ui-btn-corner-all" aria-hidden="true">
                                <span class="ui-btn-text">Login</span>
                        </span>
                        <label for="txtLogin" class="ui-hidden-accessible">Login:</label>
                        <input type="text" name="txtLogin" maxlength="50" id="txtLogin" value="pru" placeholder="Login" style="width:93%"/>
                        <input type="password" name="pasPassword" maxlength="50" id="pasPassword" value="p" placeholder="Contraseña" style="width:93%"/>
                        <button class="ui-btn-hidden" value="submit-value" name="submit" data-icon="check" data-iconpos="right" data-theme="d" type="submit" aria-disabled="false">
                            Entrar
                        </button>
                    </div>
                </form>
            </div>
            <div data-role="footer" class="ui-bar ui-bar-d" data-theme="d">
                <div data-role="controlgroup" data-type="horizontal">
                    <a href="http://www.telynet.com/" data-rel="dialog">&copy; 2012 TelyNET S.A.</a>		
                </div>		
            </div>
        </div>
<?php
if(Debug::is_messages_on()||Debug::is_php_info_on()||Debug::is_sqls_on())
{
    echo "<!-- debug -->\n<div id=\"divDebug\" >\n<center>";
    Debug::get_sqls_in_html_table();
    echo "<br />";
    Debug::get_messages_in_html_table();
    echo "<br />";
    Debug::get_php_info();
    echo "</center>\n</div><!--/debug-->";
}
?>
    </body>
</html>


