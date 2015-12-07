//
var oMtbAjax =
{
    sAjaxControllerUri : 'http://192.168.1.126:8888/proy_mtbmobile/www/index.php',
    replace_element :function(sElementId,sHtml,oFuncChange)
    {
        //Recupero el select de cp
        var sJqSelector = '#'+sElementId;
        var oJqSelect = jQuery(sJqSelector);

        //tag <span> para mostrar texto seleccionado
        var eSpanText = oJqSelect.parent().children('.ui-btn-inner.ui-btn-corner-all').children('.ui-btn-text');
        //limpio el texto que se estaba mostrando
        eSpanText.empty();
        //Aplico el texto '<select >...</select>' de remplazo obtenido con ajax
        //El html puede que venga preseleccionado asi que habria que mostrar este texto
        //en el span
        oJqSelect.replaceWith(sHtml);
        //Esta funcion rellena los tags tipo span que están sobre el control 
        if(oFuncChange!=null && oFuncChange!=undefined)
        {    
            jQuery(document).delegate(sJqSelector,"change",oFuncChange);
            var sSelectedText = jQuery(sJqSelector+' option:selected').text();
            eSpanText.text(sSelectedText);        
        }
        
    },
    
    load_codigospostales : function(oElement,sTargetElementId)
    {
        //bug(oEvent);return;
        //bug(jQuery('#det_Pais'));
        //console.debug(this); this es oMtbAjax
        var sModule = 'ajax';
        var sControllerMethod = 'get_postal_codes_by_province';
        var sElementId = '#'+oElement.id;
        var sTargetElementId = sTargetElementId || 'det_Cp';
        //bug('id en codi'+sElementId);
        //console.debug(sElementId);
        var sSelectedValue = jQuery(sElementId+' option:selected').val();
        //bug(sSelectedValue);
        jQuery.post
        (
            oMtbAjax.sAjaxControllerUri,
            {module: sModule, tab: sControllerMethod, Code_Provincia: sSelectedValue},
            function(data) 
            {
                //Esta función actualiza las etiquetas span asociadas al control
                var oFuncChange = function()
                {
                    var oJqSelect = jQuery(this);
                    var sJqSelector = '#'+oJqSelect.attr('id');
                    var eSpanText = oJqSelect.parent().children('.ui-btn-inner.ui-btn-corner-all').children('.ui-btn-text');
                    var sSelectedText = jQuery(sJqSelector+' option:selected').text();
                    eSpanText.text(sSelectedText);
                }                
                oMtbAjax.replace_element(sTargetElementId, data, oFuncChange);
            }
        );
            
    },
    
    load_manager: function(oElement, sTargetElementId)
    {
        //bug(oEvent);return;
        //bug(jQuery('#det_Pais'));
        //console.debug(this); this es oMtbAjax
        var sModule = 'ajax';
        var sControllerMethod = 'get_manager_by_vendedor';
        var sElementId = '#'+oElement.id;
        var sTargetElementId = sTargetElementId || 'Code_Manager';
        //bug('id en codi'+sElementId);
        //console.debug(sElementId);
        var sSelectedValue = jQuery(sElementId+' option:selected').val();
        //bug(sSelectedValue);
        jQuery.post
        (
            oMtbAjax.sAjaxControllerUri,
            {module: sModule, tab: sControllerMethod, Code_Vendedor: sSelectedValue},
            function(data) 
            {
                //Esta función actualiza las etiquetas span asociadas al control
                var oFuncChange = function()
                {
                    var oJqSelect = jQuery(this);
                    var sJqSelector = '#'+oJqSelect.attr('id');
                    var eSpanText = oJqSelect.parent().children('.ui-btn-inner.ui-btn-corner-all').children('.ui-btn-text');
                    var sSelectedText = jQuery(sJqSelector+' option:selected').text();
                    eSpanText.text(sSelectedText);
                }                
                oMtbAjax.replace_element(sTargetElementId, data, oFuncChange);

            }
        );
    },
    
    load_clientes: function(oElement, sTargetElementId)
    {
        //bug(oEvent);return;
        //bug(jQuery('#det_Pais'));
        //console.debug(this); this es oMtbAjax
        var sModule = 'ajax';
        var sControllerMethod = 'get_clientes_by_vendedor';
        var sElementId = '#'+oElement.id;
        var sTargetElementId = sTargetElementId || 'det_Code_Account';
        //bug('id en codi'+sElementId);
        //console.debug(sElementId);
        var sSelectedValue = jQuery(sElementId+' option:selected').val();
        //bug(sSelectedValue);
        jQuery.post
        (
            oMtbAjax.sAjaxControllerUri,
            {module: sModule, tab: sControllerMethod, Code_Vendedor: sSelectedValue},
            function(data) 
            {
                //Esta función actualiza las etiquetas span asociadas al control
                var oFuncChange = function()
                {
                    var oJqSelect = jQuery(this);
                    var sJqSelector = '#'+oJqSelect.attr('id');
                    var eSpanText = oJqSelect.parent().children('.ui-btn-inner.ui-btn-corner-all').children('.ui-btn-text');
                    var sSelectedText = jQuery(sJqSelector+' option:selected').text();
                    eSpanText.text(sSelectedText);
                }                
                oMtbAjax.replace_element(sTargetElementId, data, oFuncChange);
            }
        );
    },
    load_contactos: function(oElement, sTargetElementId)
    {
        //bug(oEvent);return;
        //bug(jQuery('#det_Pais'));
        //console.debug(this); this es oMtbAjax
        var sModule = 'ajax';
        var sControllerMethod = 'get_contacts_by_client';
        var sElementId = '#'+oElement.id;
        var sTargetElementId = sTargetElementId || 'det_Code_Contact';
        //bug('id en codi'+sElementId);
        //console.debug(sElementId);
        var sSelectedValue = jQuery(sElementId+' option:selected').val();
        //bug(sSelectedValue);
        jQuery.post
        (
            oMtbAjax.sAjaxControllerUri,
            {module: sModule, tab: sControllerMethod, Code_Account: sSelectedValue},
            function(data) 
            {
                //Esta función actualiza las etiquetas span asociadas al control
                var oFuncChange = function()
                {
                    var oJqSelect = jQuery(this);
                    var sJqSelector = '#'+oJqSelect.attr('id');
                    var eSpanText = oJqSelect.parent().children('.ui-btn-inner.ui-btn-corner-all').children('.ui-btn-text');
                    var sSelectedText = jQuery(sJqSelector+' option:selected').text();
                    eSpanText.text(sSelectedText);
                }                
                oMtbAjax.replace_element(sTargetElementId, data, oFuncChange);

            }
        );
    },
    
    load_subfamilies: function(oElement, sTargetElementId, sModuleFrom)
    {
        //bug(oEvent);return;
        //bug(jQuery('#det_Pais'));
        //console.debug(this); this es oMtbAjax
        var sModule = 'ajax';
        var sControllerMethod = 'get_subfamilies_by_family';
        var sElementId = '#'+oElement.id;
        var sTargetElementId = sTargetElementId || 'sch_Code_Subfamilia';
        var sModuleFrom = sModuleFrom || 'not defined';
        //bug('id en codi'+sElementId);
        //console.debug(sElementId);
        var sSelectedValue = jQuery(sElementId+' option:selected').val();
        //bug(sSelectedValue);
        jQuery.post
        (
            oMtbAjax.sAjaxControllerUri,
            {module: sModule, tab: sControllerMethod, Code_Family: sSelectedValue, modulefrom: sModuleFrom},
            function(data) 
            {
                //Esta función actualiza las etiquetas span asociadas al control
                var oFuncChange = function()
                {
                    var oJqSelect = jQuery(this);
                    var sJqSelector = '#'+oJqSelect.attr('id');
                    var eSpanText = oJqSelect.parent().children('.ui-btn-inner.ui-btn-corner-all').children('.ui-btn-text');
                    var sSelectedText = jQuery(sJqSelector+' option:selected').text();
                    eSpanText.text(sSelectedText);
                }                
                oMtbAjax.replace_element(sTargetElementId, data, oFuncChange);

            }
        );
    }
   
}
//
/*
 *$oSelProvincia->set_js_onchange("oMtbAjax.load_codigospostales(this);");
$(document).ready(function() {
$(function(){
    var counter = 2;
    var onChange = function(e){
        //alert('a'); //was just checking you ;)
        val = $(':selected',this).val() || '';
        if (val != ''){
            // they may be able to toggle between valid options, too.
            // let's avoid that using a class we can apply
            var c = 'alreadyMadeASelection';
            if ($(this).hasClass(c))
                return;
            // now take the current select and clone it, then rename it
            var newSelect = $(this)
                .clone()
                .attr('name','select_options_'+counter)
                .attr('id','select_options_'+counter)
                .appendTo('#select_option_groups')
                .change(onChange)
                .selectmenu();
            $(this).addClass(c);
            counter++;
        } else {
            var id = $(this).attr('name').match(/\d+$/), parent = $(this).parent();
            $(this).remove();
            
            // re-order the counter (we don't want missing numbers in the middle)
            $('select',parent).each(function(){
                var iid = $(this).attr('name').match(/\d+$/);
                if (iid>id)
                    $(this).attr('name','select_options_'+(iid-1));
            });
            counter--;
        }
    };
    $('#select_option_groups select').change(onChange);
});
});
*/