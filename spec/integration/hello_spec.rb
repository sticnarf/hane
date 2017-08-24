require 'rspec'
require 'capybara/rspec'
require 'capybara/webkit'

Capybara.configure do |config|
  config.run_server = false
  config.default_driver = :webkit
  config.app_host = ENV['APP_HOST']
end

describe 'the main website' do
  it 'shows Message board and reject users not logged in' do
    visit '/'
    expect(page).to have_content 'Message board'
    expect(page).to have_content 'You are not logged in.'
    expect(page).not_to have_content 'New message'
  end
end
