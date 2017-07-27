require 'rspec'
require 'capybara/rspec'
require 'capybara/webkit'

Capybara.configure do |config|
  config.run_server = false
  config.default_driver = :webkit
  config.app_host = ENV['APP_HOST']
end

describe 'the main website' do
  it 'shows hello' do
    visit '/'
    expect(page).to have_content 'Hello'
  end

  it 'should greet the user if name is provided' do
    visit '/?name=Alice'
    expect(page).to have_content 'Hello, Alice'
  end
end
