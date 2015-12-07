<?php
class ModelClients extends MainModel implements InterfaceModel
{
    protected $_code;
    private $_codigo_sic;
    private $_correo_e;
    private $_cp;
    private $_direccion;
    private $_fax;
    private $_name;
    private $_pais;
    private $_propietario;
    private $_provincia;
    private $_tlf1;
    private $_tlf2;
    
    //Para realizar busquedas
    private $_mtb_poblacion;
    public $oPropietario;
    
    public function __construct(
        $code_user_session="", $code="", $propietario="",$codigo_sic="", $correo_e="", $cp="", $direccion="", $fax="", 
        $name="", $pais="", $provincia="", $tlf1="", $tlf2="", $poblacion="")
    {
        global  $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "clients";
        $this->_table_code = "326";
        $this->_code_user_session = $code_user_session;
        
        $sSQL = "SELECT Code, Direccion, Pais, Tlf1, Provincia, Tlf2, Cp, Fax, Correo_E, Codigo_Sic, Propietario, Name
                 FROM clients";
        $this->_default_sql = $sSQL;
        
        $this->_code = $code;
        $this->_codigo_sic = $codigo_sic;
        $this->_correo_e = $correo_e;
        $this->_cp = $cp;
        $this->_direccion = $direccion;
        $this->_fax = $fax;
        $this->_name = $name;
        $this->_pais = $pais;
        $this->_propietario = $propietario;
        $this->_provincia = $provincia;
        $this->_tlf1 = $tlf1;
        $this->_tlf2 = $tlf2;
        $this->_mtb_poblacion = $poblacion;
        
    }

    public function load_by_code()
    {
        $sSQL = $this->_default_sql;
        $sSQL .= " WHERE Code='$this->_code'";
        //bug($sSQL,"en load by code");
        $this->load_from_sql($sSQL);
        $this->oPropietario = new ModelCoreUsers(null,$this->_propietario);
        $this->oPropietario->load_by_code();
    }
    
    public function update(){}
    
    public function delete(){}
     
    public function insert(){}
    
    protected function set_all_properties($arRow) 
    {
        $this->_code = $arRow["Code"];
        $this->_codigo_sic = $arRow["Codigo_Sic"];
        $this->_correo_e = $arRow["Correo_E"];
        $this->_cp = $arRow["Cp"];
        $this->_direccion = $arRow["Direccion"];
        $this->_fax = $arRow["Fax"];
        $this->_name = $arRow["Name"];
        $this->_pais = $arRow["Pais"];
        $this->_propietario = $arRow["Propietario"];
        $this->_provincia = $arRow["Provincia"];
        $this->_tlf1 = $arRow["Tlf1"];
        $this->_tlf2 = $arRow["Tlf2"];        
    }
    
    public function get_mobile_list()
    {
        
        $sCondSearch = "";
        //$sUrl = $this->_list_url;
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
        
        $sCodeUserSession = $this->_code_user_session;
        if(empty($sCodeUserSession))
            $sCodeUserSession = -1;

        $arValues["c.name"] = $this->_name;
        $arValues["c.codigo_sic"] = $this->_codigo_sic;
        $arValues["cp.Cp"] = $this->_cp;
        $arValues["mtc.Poblacion"] = $this->_mtb_poblacion;
        $sCondSearch = $this->build_sql_like_for_search($arValues);
        
        $sSQL = "SELECT c.Id
                FROM clients AS c
                INNER JOIN MTB_Clientes AS mtc
                ON c.Code = mtc.Code
                LEFT JOIN core_users u
                ON c.propietario = u.code
                -- PICKLIST
                LEFT JOIN MTB_Codigos_Postales AS cp
                ON c.Cp = cp.Code
                WHERE 1=1 
                AND c.Propietario IN (SELECT Code FROM get_code_vendedores('$sCodeUserSession'))";
        
        if(!empty($sCondSearch))
            $sSQL .= " AND $sCondSearch";
        
        //bug($sSQL); die;
        $arIdPaginate = $this->oDB->query($sSQL);

        //bug($arIdPaginate,$sSQL);
        $this->_list_total_regs = count($arIdPaginate);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arIdPaginate,$iPage,"",$iItemsPerPage);
        //Si la página pedida tiene un valor fuera de rango se corrige en el componente y hay que
        //actualizarla
        $this->_list_page = $oPaginate->get_pagina();
        //bug($this->_list_page);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        //bug($this->_list_num_pages);
        //bug($this->_list_arPages);
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arIdPaginate = $oPaginate->get_array_items();
        //Obtengo los valores autonumericos de la columna Id
        $arIdPaginate = $this->get_field_values($arIdPaginate, "Id");
        
        $sSQL = "SELECT c.Code, c.Name AS Nombre, mtc.Poblacion AS Poblacion, 
                cp.Code AS cp_code, cp.Cp, c.Propietario AS Code_User, 
                u.Description AS Vendedor
                FROM clients AS c
                INNER JOIN MTB_Clientes AS mtc
                ON c.Code = mtc.Code
                LEFT JOIN core_users u
                ON c.propietario = u.code
                -- PICKLIST
                LEFT JOIN MTB_Codigos_Postales AS cp
                ON c.Cp = cp.Code
                WHERE 1=1 
                AND c.Id IN ".$this->build_sqlin($arIdPaginate,true);
        //bug($arIdPaginate,$sSQL); die;
        $arData = $this->oDB->query($sSQL);
        //bug($arData,$sSQL); 
        return $arData;                
    }
    
    public function get_code(){ return $this->_code; }
    public function get_codigo_sic(){ return $this->_codigo_sic; }
    public function get_correo_e(){ return $this->_correo_e; }
    public function get_cp(){ return $this->_cp; }
    public function get_direccion(){ return $this->_direccion; }
    public function get_fax(){ return $this->_fax; }
    public function get_name(){ return $this->_name; }
    public function get_pais(){ return $this->_pais; }
    public function get_propietario(){ return $this->_propietario; }
    public function get_provincia(){ return $this->_provincia; }
    public function get_tlf1(){ return $this->_tlf1; }
    public function get_tlf2(){ return $this->_tlf2; }
    
    public function set_code($value){ $this->_code = $value; }
    public function set_codigo_sic($value){ $this->_codigo_sic = $value; }
    public function set_correo_e($value){ $this->_correo_e = $value; }
    public function set_cp($value){ $this->_cp = $value; }
    public function set_direccion($value){ $this->_direccion = $value; }
    public function set_fax($value){ $this->_fax = $value; }
    public function set_name($value){ $this->_name = $value; }
    public function set_pais($value){ $this->_pais = $value; }
    public function set_propietario($value){ $this->_propietario = $value; }
    public function set_provincia($value){ $this->_provincia = $value; }
    public function set_tlf1($value){ $this->_tlf1 = $value; }
    public function set_tlf2($value){ $this->_tlf2 = $value; }
    
    public function set_mtb_poblacion($value){ $this->_mtb_poblacion = $value; }
    public function get_mtb_poblacion(){ return $this->_mtb_poblacion; }

}
