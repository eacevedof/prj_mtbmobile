var on_anchor_click = function(event)
{
    event.preventDefault();
    var sHrefPropuesta = jQuery(this).attr('href');
    var sHrefDescuento = sHrefPropuesta.replace('propuesta','descuento');
    sHrefDescuento = sHrefDescuento.replace('detail','list');
    //bug(sHref);
    var arLinks = [];
    arLinks['familia'] = sHrefPropuesta + '&Type=familia';
    arLinks['subfamilia'] = sHrefDescuento + '&Type=subfamilia';
    arLinks['neto'] = sHrefDescuento + '&Type=neto';
    
    //bug(arLinks['familia']);return false;
    jQuery('#divDialog').simpledialog2
    (   
        {
            mode: 'blank',
            headerText: 'Propuesta',
            headerClose: true,
            //blankContent : '<ul data-role="listview"><li>li</li></ul>'
            /**/
            blankContent : 
                    '<ul data-role="listview">\n\
                        <li><a href="'+arLinks['familia']+'" onclick="false">Familia</a></li>\n\
                        <li><a href="'+arLinks['subfamilia']+'" onclick="false">Subfamilia</a></li>\n\
                        <li><a href="'+arLinks['neto']+'" onclick="false">Neto</a></li>\n\
                    </ul>\n\
                    <a rel="close" data-role="button" href="#">Cancelar</a>'
            /**/
        }
    )
}
//Evento sobre fila
jQuery(document).delegate
(
    '.clsPopup', //Este es un atributo neutro que sirve para recorrer todos los links con jquery
    'click', 
    on_anchor_click
);

