<?php
class ComponentPaginate extends MainComponent
{
    private $_arItems;
    /**
    * Los items por pagina que se deberan mostrar
    *
    * @var mixed
    */
    private $_iItemsPagina;
    private $_iPagina;
    private $_iNumPaginas;
    private $_isExacto = true;
   
    private $_sUrl;
   
    private $_isError = false;
    private $_sMensaje;

    public function __construct($arItems, $iPagina=1, $sUrl="", $iItemsPagina=15)
    {
        //sino se intenta forzar el null a 1
        $iPagina = $this->avoid_not_number($iPagina);
        $this->_iPagina = $iPagina;
        $this->_arItems = $arItems;
        $this->_iItemsPagina = $iItemsPagina;    
        $this->_sUrl = $sUrl;
        $this->_iNumPaginas = $this->num_paginas_resultantes();
        //Comprobacion de rangos.
        if($this->_iPagina <= 0 || $this->_iPagina > $this->_iNumPaginas )
        {
            $this->_isError = true;
            $this->_sMensaje = "Error: pagina fuera de rango. Pagina pedida:$iPagina, total paginas:".$this->_iNumPaginas;
            Debug::set_message($this->_sMensaje, "Paginaci—n");
            $this->fix_page_out_of_range();
        }
        
    }
   
    private function fix_page_out_of_range()
    {
        if($this->_iPagina<=0 && $this->_iNumPaginas>0)
        {
            $this->_iPagina =1;
        }
        elseif($this->_iPagina>$this->_iNumPaginas)
        {
            $this->_iPagina = $this->_iNumPaginas;
        }
    }
    
    private function avoid_not_number($string)
    {
        $iEntero = $string;
        if(!is_numeric($iEntero))
        {
            $iEntero = 1;
        }
        return $iEntero;
    }
   
    /**
    * Recupera una cantidad de items desde el array que contiene a
    * todos.  Los items de la pagina correspondiente
    */
    public function get_array_items()
    {
        $arResultado = array();
       
        $arItems = $this->_arItems;
        $iItemsPagina = $this->_iItemsPagina;
        $iPagina = $this->_iPagina;
        $iNumPaginas = $this->_iNumPaginas;

        //bug($iPagina); bug($iNumPaginas);        
        //Si la pagina recibida esta en el rango
        if($iPagina > 0 && $iPagina <= $iNumPaginas)
        {
            //iKeyInicio = 8 x (4-1) = 24
            $iKeyInicio = $iItemsPagina * ($iPagina - 1);
            //iKeyFinal = 24 + (8 - 1) = 24 + 7 = 31
            //En caso de resto == 0 y que la pagina actual no sea
            //la ultima se puede aplicar el key Final asi
            $iKeyFinal = $iKeyInicio + ($iItemsPagina - 1);
            //Si la pagina actual es la ultima
            if($iPagina == $iNumPaginas && !$this->_isExacto)
            {
                //Las paginas que respetan los items/pagina
                $iPaginasCompletas = $iNumPaginas - 1;
                //El numero de items distribuidos en estas paginas
                $iItemsDistribuidos = $iPaginasCompletas * $iItemsPagina;
                $iItemsUltimaPagina = count($arItems) - $iItemsDistribuidos;
               
                $iKeyFinal = $iKeyInicio + ($iItemsUltimaPagina - 1);  
            }

            for($i=$iKeyInicio; $i<=$iKeyFinal; $i++)
            {
                $arResultado[] = $arItems[$i];
            }
            //bug($arResultado, 'get_array_items');
        }
        else //La pagina pedida no esta en el rango
        {
            $arResultado = null;
        }
        return $arResultado;
    }
   
    /**
    * Calcula el numero de paginas en las que se puede dividir
    * el array completo
    * @return integer iNumPaginas  El numero de paginas en que se dividira $this->_arItems
    */
    private function num_paginas_resultantes()
    {
        $arItems = $this->_arItems;
        $iItemsPagina = $this->_iItemsPagina;  
        $iTotalItems = count($arItems);
         
        $iNumPaginas = intval( $iTotalItems / $iItemsPagina);
        $iResto = $iTotalItems % $iItemsPagina;
        //Si el resto es mayor que cero es porque se necesita
        //una pagina mas
        if($iResto > 0)
        {
            //Con esto indico que la ultima pagina no tendra
            //un listado respetando "itemps/pagina" sino que sera menor
            $this->_isExacto = false;
            $iNumPaginas++;
        }

        //bug($iNumPaginas);
        return $iNumPaginas;        
    }
   
    /**
    * Devuelve la lista de todos los botones !< << i, ii, ... n >> >|
    * en html
    */
    private function get_array_navegacion()
    {
        $arBotones = array();
        $iNumPaginas = $this->_iNumPaginas;
        $iPaginaActual = $this->_iPagina;
       
        //Hay al menos una pagina
        if($iNumPaginas > 0)
        {
            //En caso que se pase una pagina 0 o negativa (Fuera de rango)
            if ($iPaginaActual < 1)
            {
                //$arBotones = "Error: pagina fuera de rango";
                $arBotones = null;
                $this->_isError = true;
                $this->_sMensaje = "Error nav: pagina fuera de rango pagina actual:$iPaginaActual, total paginas:$iNumPaginas";
                $this->fix_page_out_of_range();                
            }
            //Si solo existe una unica pagina
            elseif($iPaginaActual==1 && $iNumPaginas==1)
            {
                echo "nulo paginactual es 1";  
                //No hay botones de ningun tipo
                $arBotones = null;
            }
            //Si estamos en la primera y hay mas de una
            elseif($iPaginaActual==1 && $iNumPaginas>1)
            {
                $arBotones["nav_numeros"] = $this->get_botones_numero();
                $arBotones["nav_siguiente"] = $this->get_botones_siguiente();
            }
            //Si no es ni la primera ni la ultima                        
            elseif($iPaginaActual>1 && $iPaginaActual<$iNumPaginas)
            {
                $arBotones["nav_anterior"] = $this->get_botones_anterior();
                $arBotones["nav_numeros"] = $this->get_botones_numero();
                $arBotones["nav_siguiente"] = $this->get_botones_siguiente();                
            }
            //Si es la ultima                        
            elseif ($iPaginaActual>1 && $iPaginaActual==$iNumPaginas)
            {
                $arBotones["nav_anterior"] = $this->get_botones_anterior();
                $arBotones["nav_numeros"] = $this->get_botones_numero();
            }
            //Si es mayor a la ultima (fuera de rango)                        
            elseif ($iPaginaActual > $iNumPaginas)
            {
                //$arBotones = "Error: pagina fuera de rango pagina actual:$iPaginaActual, total paginas:$iNumPaginas";
                $arBotones = null;
                $this->_isError = true;
                $this->_sMensaje = "Error nav: pagina fuera de rango pagina actual:$iPaginaActual, total paginas:$iNumPaginas";
                //Deja la pagina en un valor valido. La Ãºltima o la primera
                $this->fix_page_out_of_range();
            }
        }
        return $arBotones;        
    }
   
    /**
    * Crea un array con todos los botones que llevan un numero de pagina
    * El boton de la pagina actual se crea sin link
    */
    private function get_botones_numero()
    {
        $arBoton = array();
        $iPaginaNoBoton = $this->_iPagina;
        $iNumPaginas = $this->_iNumPaginas;
        $sUrl = $this->_sUrl;
       
        for($i=1; $i<=$iNumPaginas; $i++)
        {
            if($i != $iPaginaNoBoton)
            {
                //bug($sUrl,'url en get_botones numero');
                $sTmpUrl = $sUrl . $i; ;
                $sBotonAnchor = "<span>&nbsp;<a href=\"$sTmpUrl\" >$i</a>&nbsp;</span>";
            }
            else //La pagina con el numero que corresponde al boton
            {
                $sBotonAnchor = "<span class=\"stlPaginaActual\">$iPaginaNoBoton</span>";
            }
            $arBoton[$i] = $sBotonAnchor;
        }
       
        return $arBoton;        
    }
   
    private function get_botones_anterior()
    {
        $arBoton = array('inicio'=>"",'final'=>"");
        $iPaginaActual = $this->_iPagina;
        $iNumPaginas = $this->_iNumPaginas;
        $sUrl = $this->_sUrl;
 
        //Pagina 1
        $sTmpUrl = $sUrl . 1;
        $arBoton["inicio"] = "<span>&nbsp;<a href=\"$sTmpUrl\" >|&lt;</a>&nbsp;</span>";
       
        //Pagina anterior a la actual si la actual es > 1
        $sTmpUrl = $sUrl . ($iPaginaActual-1);
        $arBoton["anterior"] = "<span>&nbsp;<a href=\"$sTmpUrl\" >&lt;&lt;</a>&nbsp;</span>";
       
        return $arBoton;  
    }
   
    private function get_botones_siguiente()
    {
        $arBoton = array('siguiente'=>"",'final'=>"");
        $iPaginaActual = $this->_iPagina;
        $iUltimaPagina = $this->_iNumPaginas;
        $sUrl = $this->_sUrl;

        //Solo hay siguiente si la pagina_actual es menor a la ultima pagina (o total de paginas)
        if($iPaginaActual<$iUltimaPagina)
        {
            $sTmpUrl = $sUrl . ($iPaginaActual+1);
            $arBoton["siguiente"] = "<span>&nbsp;<a href=\"$sTmpUrl\" >&gt;&gt;</a>&nbsp;</span>";        
        }
       
        $sTmpUrl = $sUrl . $iUltimaPagina;
        $arBoton["final"] = "<span>&nbsp;<a href=\"$sTmpUrl\" >&gt;|</a>&nbsp;</span>";
       
        return $arBoton;          
    }
   
    /**
    * Devuelve un array indexado por enteros con los items que le corresponde
    * a la pagina $this->_iPagina
    */
    public function get_items_to_show()
    {
        return $this->get_array_items();            
    }

    /**
    * Devuelve un array tipo arBotones[anterior], arBotones[siguiente] ...
    */    
    public function get_botones_to_show()
    {
        return $this->get_array_navegacion();
    }
   
    /**
    * Devuelve los botones pero en un string. Para poder pintarlos con un echo
    */
    public function get_botones_in_html()
    {
        $sBotonesHtml = "";
        $arBotones = $this->get_array_navegacion();
        //bug($arBotones,'arbotones en get_botones_in_html');
       
        //Navegacion inicio anterior
        if(!empty($arBotones["nav_anterior"]))
        {
            $sBotonesHtml .= $arBotones["nav_anterior"]["inicio"];
            $sBotonesHtml .= $arBotones["nav_anterior"]["anterior"];
        }
        //Navegacion numeros paginas
        if(!empty($arBotones["nav_numeros"]))
        {
            foreach($arBotones["nav_numeros"] as $sBoton)
            {
                $sBotonesHtml .= $sBoton;    
            }
        }        
        //Navegacion siguiente final
        if(!empty($arBotones["nav_siguiente"]))
        {
            $sBotonesHtml.= $arBotones["nav_siguiente"]["siguiente"];
            $sBotonesHtml.= $arBotones["nav_siguiente"]["final"];
        }        
       
        return $sBotonesHtml;
    }
   
    /**
    * Determina si ha ocurrido un error al pasar la pagina.
    * Error de tipo rango
    */
    public function is_error()
    {
        return $this->_isError;
    }
   
    public function get_mensaje()
    {
        return $this->_sMensaje;
    }
    
    public function get_num_paginas()
    {
        return $this->_iNumPaginas;
    }
    
    public function get_paginas_for_select()
    {
        $arPaginas = array();
        for($i=1; $i<=$this->_iNumPaginas; $i++)
        {
            $arPaginas[$i] = "p‡gina $i";
        }
        return $arPaginas;
    }
    
    /**
     * Si se ha corregido la p‡gina por fuera de rango. Cambiaria la que se introdujo
     * en el constructor
     */
    public function get_pagina()
    {
        return $this->_iPagina;
    }
}