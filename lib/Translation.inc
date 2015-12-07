<?php
class Translation{

        private $translationTable = 'm_translations';
        private $codeField = 'Code_Text';
        private $translation_modules = 'm_module_translations';
        private $moduleField = 'Code_Module';

        # Constructor
        public function __construct($dest_lang = 'es', $module = false){	
                $this->dest_lang = $dest_lang;
                if($module) $this->translate_module($module);
        }

        public function translate($code_text){
                global $DB;

                $tmp = $DB->query("SELECT " . $this->dest_lang . " FROM " . $this->translationTable . " WHERE " . $this->codeField . " = '" . $code_text . "'");
                $this->dest_text = $tmp[0][$this->dest_lang];

                if($this->dest_text == '') $this->dest_text = '404';

                return $this->dest_text;

        }

        private function translate_module($module){
                global $DB;

                $tmp = $DB->query("SELECT " . $this->translationTable . "." . $this->codeField . ", " . $this->translationTable . "." . $this->dest_lang . " FROM " . $this->translationTable . " INNER JOIN " . $this->translation_modules . " ON " . $this->translationTable . "." . $this->codeField . " = " . $this->translation_modules . "." . $this->codeField . "  WHERE " . $this->translation_modules . "." . $this->moduleField . " = '" . $module . "' OR " . $this->translation_modules . "." . $this->moduleField . " = 'DEFAULT'");

                if($tmp) foreach($tmp as $elem)	$this->{$module}->{$elem[$this->codeField]} = $elem[$this->dest_lang];

                if(!$tmp || !is_object($this->{$module})) $this->{$module} = '404';

                return $this->{$module};

        }

}
