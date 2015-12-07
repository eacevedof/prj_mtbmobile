<!-- info_bar.php -->
<div data-role="header" data-theme="d" data-position="fixed" style="line-height:32px; height:32px;">
    &nbsp;&middot;&nbsp;<?php echo $oSite->get_today_date(); ?>&nbsp;&nbsp;&middot;&nbsp;&nbsp;
    <strong>
        <?php echo $oSite->get_project_name(); ?>
    </strong>
    <a href="index.php?user_action=exit" data-icon="delete" class="ui-btn-right" data-theme="a" data-iconpos="notext">
        <?php echo $oSite->get_logged_user_name(); ?>
    </a>
</div>
<!-- /info_bar.php -->