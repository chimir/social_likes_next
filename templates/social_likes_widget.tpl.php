<div class="social-likes <?php print $element['#look']; ?>"
  <?php if ($element['#url']): ?>
    data-url="<?php print $element['#url']; ?>"
  <?php endif; ?>
  <?php if ($element['#title']): ?>
    data-title="<?php print $element['#title']; ?>"
  <?php endif; ?>
>
  <?php foreach ($element['#websites'] as $website): ?>
    <div data-service="<?php print $website; ?>"
      <?php if ($website == 'twitter'): ?>
        <?php if ($element['#via']): ?>
          data-via="<?php print $element['#via']; ?>"
        <?php endif; ?>
        <?php if ($element['#related']): ?>
          data-related="<?php print $element['#related']; ?>"
        <?php endif; ?>
      <?php endif; ?>
      <?php if ($website == 'pinterest'): ?>
        data-media="<?php print $element['#image_url']; ?>"
      <?php endif; ?>
    >
      <?php print $element['#labels'] == FALSE ? $element['#label'][$website] : ''; ?>
    </div>
  <?php endforeach; ?>
</div>